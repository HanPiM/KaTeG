#include "serializer.hpp"
#include "s_predef.h"

std::string kcolor_to_string(unsigned int hex_rgb)
{
	// 0xAABBCC -> #ABC
	bool flag = 0;
	unsigned hex = hex_rgb;

	if (!(hex & 0xff000000))
	{
		flag = (hex & 0xf) == ((hex >> 4) & 0xf)
			&& ((hex >> 8) & 0xf) == ((hex >> 12) & 0xf)
			&& ((hex >> 16) & 0xf) == ((hex >> 20) & 0xf);
	}

	if (flag)
		hex = ((hex & 0xf0000) >> 8) | ((hex & 0xf00) >> 4) | (hex & 0xf);

	if (hex == kcolor::transparent)
	{
		return "transparent";
	}
	else
	{
		std::stringstream ss;
		ss.fill('0');

		if (flag)ss << '#' << std::setw(3);
		else ss << std::setw(6);

		ss << std::hex << hex;
		return ss.str();
	}
}

kitem_size serializer::item_size(kitem_size siz)
{
	kitem_size tmp = _item_size;
	_item_size = siz;

	static constexpr const char* name_of[] =
	{
		"normalsize",
		"tiny","scriptsize","footnotesize","small",
		"large","Large","LARGE","huge","Huge"
	};

	do_last_todo_op();

	_add_char_nocheck('\\');
	_add_str_nocheck(name_of[static_cast<uint32_t>(siz)]);

	return tmp;
}

void serializer::do_last_todo_op()
{
	if (no_todo_op())return;
	if (_last_todo_op_type() == _todo_type::close_op)return;

	_do_last_todo_op_nocheck();
}

void serializer::end_context()
{
	while (!no_todo_op())
	{
		if (_last_todo_op_type() == _todo_type::close_op)
		{
			_do_last_todo_op_nocheck();
			break;
		}
		_do_last_todo_op_nocheck();
	}
}

void serializer::end_all_context()
{
	while (!no_todo_op())
	{
		end_context();
	}
}

void serializer::color(unsigned int hex_rgb)
{
	_todo_func_t f = [hex_rgb, this](const _todo_info& info)
	{
		add_str("\\color{");
		add_str(kcolor_to_string(hex_rgb));
		add_char('}');
	};

	if (_last_todo_op_type() == _todo_type::color)
		_todos.top().first = f;
	else
	{
		do_last_todo_op();
		_push_todo_op(_todo_type::color, f, nullptr);
	}

	_cur_color = hex_rgb;
}

void serializer::kern(kunit x)
{
	if (_last_todo_op_type() == _todo_type::kern)
	{
		_last_todo_info().data_of<_todo_type::kern>() += x;
	}
	else
	{
		do_last_todo_op();
		_push_todo_op(_todo_type::kern, [this](const _todo_info& info)
			{
				auto& v = info.data_of<_todo_type::kern>();
				if (v != 0)_os << "\\kern{" << v << "}";
			}, x
		);
	}
}

void serializer::rule(kunit w, kunit h, kunit offset)
{
	if (w == 0)return;
	do_last_todo_op();

	_os << "\\rule";
	if (offset != 0)
	{
		_os << '[' << offset << ']';
	}
	_os << '{' << w << "}{" << h << '}';
}

void serializer::begin_empty_context(char beg, char end)
{
	do_last_todo_op();
	_os << beg;
	_push_todo_op(_todo_type::close_op, [this, end](const _todo_info&) {_os << end; }, nullptr);
}

void serializer::begin_raisebox_context(kunit x)
{
	do_last_todo_op();
	if (x != 0)
	{
		_os << "\\raisebox{" << x << "}";
	}
	begin_empty_context();
	if (x != 0)
	{
		if ((mode() & serializer_mode::inherit_item_size) &&
			item_size() != kitem_size::normal
		) item_size(item_size());
	}
}

static constexpr kunit boxed_offset_of[] =
{
	2.99,

	1.49, 2.1, 2.4, 2.7, // 100 误差 0.1 以内

	3.6, 4.31, 5.18,

	6.43,

	7.46 // Huge 100 误差 0.1 以内 测试函数 sin(x) [1,100] step=1
};

void serializer::begin_raisemath_context(kunit x)
{
	do_last_todo_op();
	if (x == 0)
	{
		begin_empty_context();
		return;
	}
	if(mode() & serializer_mode::no_def)
	{
		_os << "\\raisebox{" << x << "}{";
		try_begin_math_context();
		_push_todo_op(_todo_type::close_op, [this](const _todo_info& info)
			{
				end_context();
				_os << '}';
			}, nullptr
		);
	}
	else
	{
		kunit xoffset_of_fake_ctx = 0;
		if (!has_predef(PREDEF_RAISEMATH_CTX))
		{
			BEGIGN_PREDEF_ARG(*this, PREDEF_RAISEMATH_CTX);
			// #1 yoffset #2 info
			tmps.add_str("#1#2{\\raisebox{#1}{");
			//tmps.try_begin_math_context(true);
			if (tmps.support_fake_math_context())
			{
				tmps.add_str("\\color{transparent}\\boxed{#2}");
			}
			else tmps.add_str("$#2$");

			tmps.add_char('}');
			END_PREDEF(*this, PREDEF_RAISEMATH_CTX);
		}
		call_predef(PREDEF_RAISEMATH_CTX);
		add_char('{');
		_os << x << '}';
		
		if (support_fake_math_context())
		{
			add_char('{');
			color(color());
			kern(-boxed_offset_of[static_cast<uint32_t>(_item_size)]);
			_push_todo_op(
				_todo_type::close_op,
				[this](const _todo_info&) {
					kern(-boxed_offset_of[static_cast<uint32_t>(_item_size)]);
					add_char('}');
				},nullptr
			);
		}
		else begin_empty_context();
	}

	if ((mode() & serializer_mode::inherit_item_size) &&
		item_size() != kitem_size::normal
		) item_size(item_size());
}

void serializer::begin_x_overlapping_context(kalign_style align_style)
{
	do_last_todo_op();
	_add_str_nocheck("\\math");
	char ch = 0;

	// mathllap 这里的 l,c,r 指的是 向 哪个方向排布
	// 这里反过来即变成向哪个方向对齐

	switch (align_style)
	{
	case kalign_style::left:
		ch = 'r';
		break;
	case kalign_style::center:
		ch = 'c';
		break;
	case kalign_style::right:
		ch = 'l';
		break;
	}
	_add_char_nocheck(ch);
	_add_str_nocheck("lap");
	begin_empty_context();
}

void serializer::begin_y_overlapping_context()
{
	do_last_todo_op();
	_add_str_nocheck("\\smash");
	begin_empty_context();
}

void serializer::begin_fake_math_context()
{
	if (mode() & serializer_mode::for_discuss)return;

	/* debug code:
	$
	\def\flag{\mathrlap{\rule[-5pt]{0.5pt}{30pt}}}
	\def\offset{7.4pt}
	\Huge a\flag x\flag sgaah\flag Z\flag
	\\
	a\flag
	\raisebox{}{\boxed{
		\kern{-\offset}
		x\flag sgaah
		\kern{-\offset}
	}}\flag Z
	\flag
	$
	*/

	auto old_col = color();
	_todo_func_t f = [this](const _todo_info&)
	{
		add_char('}');
		kern(-boxed_offset_of[static_cast<uint32_t>(_item_size)]);
	};

	if (mode() & serializer_mode::no_def)
	{
		color(kcolor::transparent);
		add_str("\\boxed{");
	}
	else
	{
		if (!has_predef(PREDEF_FAKE_MATH_CTX))
		{
			BEGIGN_PREDEF_ARG(*this, PREDEF_FAKE_MATH_CTX);
			// #1 info
			_outbuf << "#1{";
			tmps.color(kcolor::transparent);
			tmps.add_str("\\boxed{#1}");
			END_PREDEF(*this, PREDEF_FAKE_MATH_CTX);
		}
		call_predef(PREDEF_FAKE_MATH_CTX);
		_os << '{';
	}

	_push_todo_op(_todo_type::close_op, f, nullptr);

	kern(-boxed_offset_of[static_cast<uint32_t>(_item_size)]);
	color(old_col);
}

void serializer::begin_inline_math_context()
{
	if (mode() & serializer_mode::for_inline)return;
	begin_empty_context('$', '$');
}

bool serializer::has_predef(const std::string& key) const
{
	return _pre_def.find(key) != _pre_def.end();
}

kunit serializer::run(const draw_func_t& func)
{
	auto w = func(*this);
	kern(-w);
	return w;
}
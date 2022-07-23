#pragma once

#include <stack>
#include <string>
#include <variant>

#include <functional>
#include <utility>

#include <iostream>
#include <sstream>

#include <unordered_map>

#include <exception>

#include "ktype.hpp"

struct serializer_mode
{
	enum
	{
		no_def = 1,
		for_discuss = (1 << 2),
		for_inline = (1 << 3),
		inherit_item_size = (1 << 4),
		discuss_mode = for_discuss | no_def | inherit_item_size
	};
};

class serializer
{
public:

	serializer(uint32_t mode=0,kitem_size siz=kitem_size::normal) :
		_mode(mode), _item_size(siz),_cur_color(0){}
	~serializer()
	{
		
	}

	inline uint32_t mode()const { return _mode; }
	uint32_t mode(uint32_t m)
	{
		auto tmp = _mode;
		_mode = m;
		return _mode;
	}

	inline kitem_size item_size()const { return _item_size; }
	kitem_size item_size(kitem_size siz);

	void output_to(std::ostream& os)const
	{
		if (_mode & serializer_mode::no_def) {}
		else if(!_pre_def.empty())
		{
			os << "% [" << _pre_def.size() << " pre_definition]\n";
			for (const auto& it : _pre_def)
			{
				os << it.second;
			}
			os << "\n% [main_body]\n";
		}
		os << _os.str();
	}

	void output_to(std::string& s)const
	{
		std::stringstream ss;
		output_to(ss);
		s += ss.str();
	}
	

	void add_str(const std::string& s)
	{
		_do_last_todo_op(); _add_str_nocheck(s);
	}
	void add_char(char ch)
	{
		_do_last_todo_op(); _add_char_nocheck(ch);
	}
	void call_predef(const std::string& key)
	{
		auto it = _pre_def.find(key);
		if (it != _pre_def.end())
		{
			add_char('\\');
			_add_str_nocheck(it->first);
		}
	}

	inline unsigned int color()const noexcept{ return _cur_color; }
	void color(unsigned int hex_rgb);
	inline void color(unsigned char r, unsigned char g, unsigned char b)
	{
		color((r << 16) | (g << 8) | b);
	}

	void kern(kunit x);
	void rule(kunit w, kunit h, kunit offset = 0_pt);

	/* context */

	void end_context();
	void end_all_context();

	/* 写入 beg 并在环境结束时写入 end */
	void begin_empty_context(char beg = '{', char end = '}');

	/* 如果 x == 0 该函数等价于 begin_empty_context */
	void begin_raisebox_context(kunit x);
	void begin_raisemath_context(kunit x);

	/**
	 * 横向重叠环境（消除横向碰撞箱）
	 * for_discuss 模式下超高部分会下移
	 * 
	 * \param align_style 向哪个方向对齐
	 */
	void begin_x_overlapping_context(kalign_style align_style = kalign_style::left);
	void begin_y_overlapping_context();

	// 用 boxed 和 overlapping 制造出数学模式
	// for_discuss 模式下不可用
	void begin_fake_math_context();

	// $...$
	// for_inline 模式下不可用
	void begin_inline_math_context();

	bool support_math_context()const
	{
		return !(
			(_mode & serializer_mode::for_discuss) &&
			(_mode & serializer_mode::for_inline)
		);
	}
	bool support_fake_math_context()const
	{
		return !(_mode & serializer_mode::for_discuss);
	}
	void try_begin_math_context(bool no_color_op = false/*for fake mode*/)
	{
		if (_mode & serializer_mode::for_discuss)
		{
			if (_mode & serializer_mode::for_inline)
			{
				throw(std::logic_error("cur_mode not support math_mode on text_mode"));
			}
			begin_inline_math_context();
		}
		else
		{
			begin_fake_math_context();
			if (no_color_op)_todos.pop();
		}
	}

	void add_predef(const std::string& key, const std::string& def)
	{
		_pre_def[key] = def;
	}
	bool has_predef(const std::string& key)const;

	using draw_func_t = std::function<kunit(serializer&)>;
	kunit run(const draw_func_t& func);

	serializer& operator<<(const draw_func_t& func)
	{
		func(*this);
		return *this;
	}

private:

	enum class _todo_type
	{
		empty_flag,

		close_op,

		add_str,
		color,
		kern
	};

	class _todo_info
	{
	public:

		template<typename _t>
		_todo_info(_todo_type t, const _t& val = nullptr)
			:_type(t), _data(val)
		{
			
		}

		_todo_type type()const
		{
			return _type;
		}
		template<_todo_type _idx>
		auto& data_of()
		{
			return std::get<static_cast<size_t>(_idx)>(_data);
		}
		template<_todo_type _idx>
		const auto& data_of()const
		{
			return std::get<static_cast<size_t>(_idx)>(_data);
		}

	private:

		_todo_type _type;
		std::variant<
			nullptr_t,
			std::monostate, std::monostate,
			unsigned int,
			kunit
		> _data;
	};

	using _todo_func_t = std::function<void(const _todo_info&)>;

	std::stack<std::pair<_todo_func_t, _todo_info> > _todos;

	inline bool _no_todo_op()const { return _todos.empty(); }

	void _do_last_todo_op();

	static constexpr const char* _name_of(_todo_type t)
	{
		switch (t)
		{
		case serializer::_todo_type::empty_flag:return "empty_flag";
			break;
		case serializer::_todo_type::close_op:return "close_op";
			break;
		case serializer::_todo_type::add_str:return "add_str";
			break;
		case serializer::_todo_type::color:return "color";
			break;
		case serializer::_todo_type::kern:return "kern";
			break;
		default:
			break;
		}
		return "?";
	}

	void _add_str_nocheck(const std::string& s) { _os << s; }
	void _add_char_nocheck(char ch) { _os << ch; }

	void _do_last_todo_op_nocheck()
	{
		decltype(_todos)::value_type tmp = _todos.top(); // avoid stupid tips
		_todos.pop();

		tmp.first(tmp.second);
	}

	template<typename _t>
	void _push_todo_op(_todo_type t, _todo_func_t f, const _t& data)
	{
		_do_last_todo_op();
		_todos.push(std::make_pair(f, _todo_info(t, data)));
	}
	_todo_type _last_todo_op_type()
	{
		return _no_todo_op()
			? _todo_type::empty_flag
			: _todos.top().second.type();
	}
	_todo_info& _last_todo_info()
	{
		return _todos.top().second;
	}

	uint32_t _mode;
	kitem_size _item_size;

	std::ostringstream _os;

	unsigned int _cur_color;

	std::unordered_map<std::string, std::string> _pre_def;
};

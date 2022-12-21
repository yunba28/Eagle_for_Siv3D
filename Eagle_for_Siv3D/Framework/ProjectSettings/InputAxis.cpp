#include "InputAxis.hpp"

namespace eagle::Internal
{
	InputAxis_impl& InputAxis_impl::add(const Node& _node)
	{
		if (not mInputs.includes(_node))
		{
			mInputs << std::move(_node);
		}

		return *this;
	}

	InputAxis_impl& InputAxis_impl::add(const Input& _input, double _scale)
	{
		Node node{ _input,_scale };

		if (not mInputs.includes(node))
		{
			mInputs << std::move(node);
		}

		return *this;
	}

	void InputAxis_impl::remove(const Input& _input)
	{
		mInputs.remove_if([&_input](const Node& node) {return node.input == _input; });
	}

	double InputAxis_impl::scale() const
	{
		double scale{ 0 };
		for (const auto& node : mInputs)
		{
			if (node.input.pressed())
			{
				scale += node.scale;
			}
		}
		return scale;
	}

	bool InputAxis_impl::Node::operator==(const Node& _other) const
	{
		return input == _other.input;
	}

	InputAxis_impl& InputAxis_helper::add(const String& _name, const InputAxis_impl::Node& _node)
	{
		return mInputAxisTable[_name].add(_node);
	}

	InputAxis_impl& InputAxis_helper::add(const String& _name, const Input& _input, double _scale)
	{
		return mInputAxisTable[_name].add(_input, _scale);
	}

	void InputAxis_helper::remove(const String& _name)
	{
		if (mInputAxisTable.contains(_name))
		{
			mInputAxisTable.erase(_name);
		}
	}

	InputAxis_impl& InputAxis_helper::get(const String& _name)
	{
		return mInputAxisTable.at(_name);
	}

	double InputAxis_helper::operator[](const String& _name) const
	{
		if (mInputAxisTable.contains(_name))
		{
			return mInputAxisTable.at(_name).scale();
		}
		return 0.0;
	}
}

namespace eagle
{
	template<>
	bool Load<Internal::InputAxis_helper>(const String& path, Internal::InputAxis_helper& value)
	{
		if (FileSystem::Extension(path) != U"iax")
			return false;

		INI ini{ path };

		if (ini.isEmpty())
			return false;

		for (const auto& section : ini.sections())
		{
			for (const auto& key : section.keys)
			{
				value.add(section.section, Parse<Internal::InputAxis_impl::Node>(key.value));
			}
		}

		return true;
	}

	template<>
	bool Save<Internal::InputAxis_helper>(const String& path, Internal::InputAxis_helper& value)
	{
		String out{ path };

		if (FileSystem::Extension(out) != U"iax")
		{
			auto it = std::find(out.begin(),out.end(),U'.');
			out.erase(it + 1, out.end());
			out += U"iax";
		}

		INI ini{};

		for (const auto& [name,axis] : value.mInputAxisTable)
		{
			ini.addSection(name);
			for (int32 i = 0;i < axis.mInputs.size();++i)
			{
				ini.write(name, U"Node{}"_fmt(i), Format(axis.mInputs[i]));
			}
		}

		return ini.save(out);
	}
}



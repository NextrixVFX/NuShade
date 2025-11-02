#pragma once

namespace NuShade::File
{
	class c_ShaderData {
	private:
		std::ifstream shaderInput;
		std::string parsedShader;

		auto OpenFile(std::ifstream& buffer, std::string_view input) -> bool
		{
			buffer.open(static_cast<std::string>(input));

			if (!buffer.is_open())
			{
				std::cerr << "Error opening file!" << std::endl << input << std::endl;
				return false;
			}

			return true;
		}

		auto ParseFile(std::ifstream& buffer, std::string& output) -> std::string
		{
			if (!buffer.is_open())
				return {};

			std::string _line;
			while (std::getline(buffer, _line))
			{
				output += _line + "\n";
			}
			
			return output;
		}

		auto CloseFile(std::ifstream& buffer) -> void
		{
			buffer.close();
		}

	public:
		
		auto GetLines(std::string& buffer) -> bool
		{
			if (!parsedShader.empty())
			{
				buffer = parsedShader;
				return true;
			}

			return false;
		}

		c_ShaderData(std::string_view input)
		{
			if (input.empty())
				return;

			if (!OpenFile(shaderInput, input))
				return;

			if (ParseFile(shaderInput, parsedShader).empty())
				return;
		}

		~c_ShaderData()
		{
			CloseFile(shaderInput);
		}
	};
}

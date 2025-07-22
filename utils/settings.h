#pragma once
namespace settings 
{

	struct ESP 
	{
		inline static bool enabled = true;
		inline static bool enemy = true;
		inline static bool name = true;
		inline static bool health = true;
		inline static bool dot = false;
		inline static bool extraUnitCheck = true;
		inline static bool heroCheck = true;
		inline static bool fairfightScreenshot = true;
		inline static ImColor enemyOccludedColor = ImColor(255, 31, 31, 255);      // Default: red
		inline static ImColor enemyVisibleColor = ImColor(12, 157, 0, 255);        // Default: green
	};

}

namespace cfg
{
	extern std::string path;
	extern std::vector<std::string> list;
	extern int currCfg;

	template<typename Type>
	void try_val(nlohmann::ordered_json& j, std::string name, Type& _value);

	void try_color(nlohmann::ordered_json& j, const std::string& name, ImColor& color);

	bool refresh();
	bool load(const std::string& name);
	bool save(std::string name);
	bool remove(std::string name);
}

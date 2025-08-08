#include "includes.h"

namespace cfg
{
    // Use %APPDATA%\KeefSWBF2 for config storage, with safe fallback
    std::string path = []() {
        const char* appData = std::getenv("APPDATA");
        if (appData && appData[0] != '\0')
            return std::string(appData) + "\\KeefSWBF2";
    }();

    std::vector<std::string> list;
    int currCfg = 0;

    template<typename Type>
    void try_val(nlohmann::ordered_json& j, std::string name, Type& _value)
    {
        if (name.empty())
            return;
        if (j.dump().find(name) == std::string::npos)
            return;
        j.at(name).get_to(_value);
    }

    void try_color(nlohmann::ordered_json& j, const std::string& name, ImColor& color)
    {
        if (j.contains(name) && j[name].is_array() && j[name].size() == 4)
        {
            color = ImColor(j[name][0].get<int>(), j[name][1].get<int>(), j[name][2].get<int>(), j[name][3].get<int>());
        }
    }

    bool refresh()
    {
        try {
            printf("Config directory path: %s\n", path.c_str());
            if (!std::filesystem::exists(path))
                std::filesystem::create_directories(path);

            list.clear();
            currCfg = 0;

            for (auto& file : std::filesystem::directory_iterator(path))
            {
                std::string file_name = file.path().filename().string();

                if (size_t location = file_name.find(".json"); location != std::string::npos)
                    file_name.replace(file_name.begin() + location, file_name.end(), "");

                if (!file_name.empty())
                    list.push_back(file_name);

                printf("%s\n", file_name.c_str());
            }

            // Force creation of Default.json if it doesn't exist
            std::string default_cfg_path = path + "\\Default.json";
            if (!std::filesystem::exists(default_cfg_path)) {
                printf("Default.json not found, creating...\n");
                save("Default");
            }
        } catch (const std::exception& e) {
            printf("Exception in refresh: %s\n", e.what());
            return false;
        }
        return true;
    }

    bool load(const std::string& name)
    {
        if (name.empty())
            return false;

        auto _path = std::string(path) + "\\" + name + ".json";

        try {
            if (!std::filesystem::exists(_path))
                return false;

            std::ifstream read_stream(_path);
            if (!read_stream.is_open())
                return false;

            nlohmann::ordered_json j;
            read_stream >> j;
            read_stream.close();

            // ESP Settings
            try_val(j, "ESP_enabled", settings::ESP::enabled);
            try_val(j, "ESP_enemy", settings::ESP::enemy);
            try_val(j, "ESP_cornerBox", settings::ESP::cornerBox);
            try_val(j, "ESP_name", settings::ESP::name);
            try_val(j, "ESP_health", settings::ESP::health);
            try_val(j, "ESP_healthNumber", settings::ESP::healthNumber);
			try_val(j, "ESP_distance", settings::ESP::distance);
            try_val(j, "ESP_dot", settings::ESP::dot);
            try_val(j, "ESP_extraUnitCheck", settings::ESP::extraUnitCheck);
            try_val(j, "ESP_heroCheck", settings::ESP::heroCheck);
            try_val(j, "ESP_fairfightScreenshot", settings::ESP::fairfightScreenshot);
			// ESP Colors
            try_color(j, "ESP_enemyVisibleColor", settings::ESP::enemyVisibleColor);
            try_color(j, "ESP_enemyOccludedColor", settings::ESP::enemyOccludedColor);
            try_color(j, "ESP_heroVisibleColor", settings::ESP::heroVisibleColor);
            try_color(j, "ESP_heroOccludedColor", settings::ESP::heroOccludedColor);
            try_color(j, "ESP_extraUnitVisibleColor", settings::ESP::extraUnitVisibleColor);
            try_color(j, "ESP_extraUnitOccludedColor", settings::ESP::extraUnitOccludedColor);


            cfg::refresh();
        } catch (const std::exception& e) {
            printf("Exception in load: %s\n", e.what());
            return false;
        }

        return true;
    }

    bool save(std::string name)
    {
        if (name.empty())
            return false;

        auto _path = std::string(path) + "\\" + name + ".json";

        nlohmann::ordered_json j = nlohmann::ordered_json
        {
            {"ESP_enabled", settings::ESP::enabled},
            {"ESP_enemy", settings::ESP::enemy},
            {"ESP_cornerBox", settings::ESP::cornerBox},
            {"ESP_name", settings::ESP::name},
            {"ESP_health", settings::ESP::health},
            {"ESP_healthNumber", settings::ESP::healthNumber},
			{"ESP_distance", settings::ESP::distance},
            {"ESP_dot", settings::ESP::dot},
            {"ESP_extraUnitCheck", settings::ESP::extraUnitCheck},
            {"ESP_heroCheck", settings::ESP::heroCheck},
            {"ESP_fairfightScreenshot", settings::ESP::fairfightScreenshot},
            {"ESP_enemyVisibleColor", {
                (int)(settings::ESP::enemyVisibleColor.Value.x * 255.0f),
                (int)(settings::ESP::enemyVisibleColor.Value.y * 255.0f),
                (int)(settings::ESP::enemyVisibleColor.Value.z * 255.0f),
                (int)(settings::ESP::enemyVisibleColor.Value.w * 255.0f)
            }},
            {"ESP_enemyOccludedColor", {
                (int)(settings::ESP::enemyOccludedColor.Value.x * 255.0f),
                (int)(settings::ESP::enemyOccludedColor.Value.y * 255.0f),
                (int)(settings::ESP::enemyOccludedColor.Value.z * 255.0f),
                (int)(settings::ESP::enemyOccludedColor.Value.w * 255.0f)
            }},
            {"ESP_heroVisibleColor", {
                (int)(settings::ESP::heroVisibleColor.Value.x * 255.0f),
                (int)(settings::ESP::heroVisibleColor.Value.y * 255.0f),
                (int)(settings::ESP::heroVisibleColor.Value.z * 255.0f),
                (int)(settings::ESP::heroVisibleColor.Value.w * 255.0f)
            }},
            {"ESP_heroOccludedColor", {
                (int)(settings::ESP::heroOccludedColor.Value.x * 255.0f),
                (int)(settings::ESP::heroOccludedColor.Value.y * 255.0f),
                (int)(settings::ESP::heroOccludedColor.Value.z * 255.0f),
                (int)(settings::ESP::heroOccludedColor.Value.w * 255.0f)
            }},
            {"ESP_extraUnitVisibleColor", {
                (int)(settings::ESP::extraUnitVisibleColor.Value.x * 255.0f),
                (int)(settings::ESP::extraUnitVisibleColor.Value.y * 255.0f),
                (int)(settings::ESP::extraUnitVisibleColor.Value.z * 255.0f),
                (int)(settings::ESP::extraUnitVisibleColor.Value.w * 255.0f)
            }},
            {"ESP_extraUnitOccludedColor", {
                (int)(settings::ESP::extraUnitOccludedColor.Value.x * 255.0f),
                (int)(settings::ESP::extraUnitOccludedColor.Value.y * 255.0f),
                (int)(settings::ESP::extraUnitOccludedColor.Value.z * 255.0f),
                (int)(settings::ESP::extraUnitOccludedColor.Value.w * 255.0f)
            }}
        };

        try {
            printf("Config directory path: %s\n", path.c_str());
            if (!std::filesystem::exists(path))
                std::filesystem::create_directories(path);

            std::ofstream output(_path);
            if (!output.is_open()) {
                printf("Failed to open file for writing: %s\n", _path.c_str());
                return false;
            }
            output << j.dump(4);
            output.close();

            if (!std::filesystem::exists(_path))
                return false;
        } catch (const std::exception& e) {
            printf("Exception in save: %s\n", e.what());
            return false;
        }

        return true;
    }

    bool remove(std::string name)
    {
        if (name.empty())
            return false;

        auto _path = std::string(path) + "\\" + name + ".json";

        try {
            if (!std::filesystem::exists(_path))
                return false;

            std::filesystem::remove(_path);
            cfg::refresh();
        } catch (const std::exception& e) {
            printf("Exception in remove: %s\n", e.what());
            return false;
        }

        return true;
    }
}
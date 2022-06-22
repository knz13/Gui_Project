#pragma once
#include <unordered_map>
#include <vector>

enum class ConsoleImportance {
	LOG = 0,
	WARN = 1,
	ERROR = 2
};


namespace GuiLayer {

	struct ConsoleMessage {
		std::string message;
		ConsoleImportance importance;
		std::string time;
	};

	class ConsoleView {
	public:
		static void PushText(ConsoleImportance imp,std::string text) {

			std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			std::string s(30,' ');
			std::strftime(&s[0], s.capacity(), "%Y-%m-%d %H:%M", std::localtime(&now));

			s.erase(s.find('\0'));

			m_Texts.push_back({ text,imp,s});

		}

		static std::vector<ConsoleMessage>& GetTexts() {
			return m_Texts;
		};


	private:
		static inline std::vector<ConsoleMessage> m_Texts;

	};
}
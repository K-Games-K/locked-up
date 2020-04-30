#pragma once

#include "notepad.hpp"
#include "widget.hpp"
#include "textured_panel.hpp"
#include "checkbox.hpp"

namespace Ui
{
    class NotepadWidget : public TexturedPanel
    {
    private:
        const std::vector<std::string> hours {
            "17:00", "17:30", "18:00", "18:30", "19:00", "19:30", "20:00",
            "20:30", "21:00", "21:30", "22:00", "22:30", "23:00"
        };

        Notepad notepad;

        int current_note = 0;
        std::vector<Ui::Panel*> note_panels;

    public:
        NotepadWidget(const Notepad& notepad, const sf::Texture& background_texture,
            const sf::Font& font, CheckboxSettings checkbox_settings, sf::Vector2f position,
            Anchor origin, Anchor anchor);

        Notepad& get_notepad();

        const Notepad& get_notepad() const;
    };
}

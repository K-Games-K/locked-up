#pragma once

#include "widget.hpp"
#include "panel.hpp"
#include "checkbox.hpp"
#include "../notepad.hpp"

namespace Ui
{
    class NotepadWidget : public Panel
    {
    public:
        NotepadWidget(const Notepad& notepad, const sf::Texture& background_texture,
            const sf::Font& font);

        NotepadWidget(const NotepadWidget& other);

        Notepad& get_notepad();

        const Notepad& get_notepad() const;

    private:
        const std::vector<std::string> hours {
            "17:00", "17:30", "18:00", "18:30", "19:00", "19:30", "20:00",
            "20:30", "21:00", "21:30", "22:00", "22:30", "23:00"
        };

        Notepad notepad;

        int current_note = 0;
        std::vector<Panel*> note_panels;

        void switch_note(int note);

        NotepadWidget* clone() const override;
    };
}

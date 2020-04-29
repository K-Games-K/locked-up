#pragma once

#include "notepad.hpp"
#include "widget.hpp"

namespace Ui
{
    class NotepadWidget : public TexturedPanel
    {
    private:
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

#include "ui/notepad_widget.hpp"

namespace Ui
{
    NotepadWidget::NotepadWidget(const Notepad& notepad, const sf::Texture& background_texture,
        const sf::Font& font, CheckboxSettings checkbox_settings, sf::Vector2f position,
        Anchor origin, Anchor anchor)
        : TexturedPanel(background_texture, position, origin, anchor),
        notepad(notepad)
    {
        auto& players = notepad.get_players();
        float button_width = get_size().x / (players.size() + 1);
        note_panels.reserve(players.size());
        for(int i = 0; i < players.size(); ++i)
        {
            add_widget(
                new Button(
                    players[i].get_nickname(),
                    font,
                    [this, i](Button&) {
                        note_panels[current_note]->set_enabled(false);
                        current_note = i;
                        note_panels[current_note]->set_enabled(true);
                    },
                    {(i + 1) * button_width, 50},
                    {button_width, 40},
                    {
                        sf::Color::Transparent,
                        sf::Color(0, 0, 0, 100),
                        sf::Color(0, 0, 0, 200)
                    },
                    {sf::Color::Black},
                    Anchor::CenterTop
                )
            );

            auto note_panel = new Ui::Panel(
                {0, -30},
                get_size() - sf::Vector2f(100, 100),
                sf::Color::Transparent,
                Anchor::Center, Anchor::Center
            );
            note_panel->set_enabled(false);
            add_widget(note_panel);
            note_panels.push_back(note_panel);
        }

        note_panels[current_note]->set_enabled(true);
    }

    Notepad& NotepadWidget::get_notepad()
    {
        return notepad;
    }

    const Notepad& NotepadWidget::get_notepad() const
    {
        return notepad;
    }
}

#include <iostream>
#include "ui/notepad_widget.hpp"
#include "ui/table_widget.hpp"

namespace Ui
{
    NotepadWidget::NotepadWidget(const Notepad& notepad, const sf::Texture& background_texture,
        const sf::Font& font, CheckboxSettings checkbox_settings, sf::Vector2f position,
        Anchor origin, Anchor anchor)
        : TexturedPanel(background_texture, position, origin, anchor),
        notepad(notepad)
    {
        auto& players = notepad.get_players();
        const int alibi_length = notepad.get_notes(players[0]).size();
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

            auto table_widget = new Ui::TableWidget(
                3, alibi_length + 1,
                {150, 300, 100},
                std::vector<float>(alibi_length + 1, 45),
                {0, 150},
                {},
                Anchor::CenterTop, Anchor::CenterTop
            );
            note_panel->add_widget(table_widget);

            table_widget->add_widget(
                0, 0, new Ui::Text(
                    "Time",
                    font,
                    {},
                    {sf::Color::Black, 26},
                    Anchor::Center, Anchor::Center
                )
            );
            table_widget->add_widget(
                1, 0, new Ui::Text(
                    "Room",
                    font,
                    {},
                    {sf::Color::Black, 26},
                    Anchor::Center, Anchor::Center
                )
            );
            table_widget->add_widget(
                2, 0, new Ui::Text(
                    "Sure",
                    font,
                    {},
                    {sf::Color::Black, 26},
                    Anchor::Center, Anchor::Center
                )
            );

            auto& notes = notepad.get_notes(players[i]);
            for(int j = 0; j < notes.size(); ++j)
            {
                table_widget->add_widget(
                    0, j + 1, new Ui::Text(
                        hours.at(j),
                        font,
                        {},
                        {sf::Color::Black},
                        Anchor::Center, Anchor::Center
                    )
                );
                auto text_edit = new Ui::TextEdit(
                    font,
                    {0, 0},
                    {290, 40},
                    {
                        sf::Color::Transparent,
                        sf::Color::Transparent,
                        sf::Color::Black
                    },
                    24,
                    Anchor::Center, Anchor::Center
                );
                text_edit->get_text().set_string(notes[j].room_name);
                table_widget->add_widget(1, j + 1, text_edit);
                table_widget->add_widget(
                    2, j + 1, new Ui::Checkbox(
                        {0, 0}, {30, 30},
                        {
                            sf::Color::Transparent,
                            sf::Color::Black,
                            1,
                            sf::Color::Black,
                            2
                        },
                        Anchor::Center, Anchor::Center
                    )
                );
            }

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

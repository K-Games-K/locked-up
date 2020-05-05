#include <iostream>
#include <ui/notepad_widget.hpp>

#include "ui/widgets.hpp"

namespace Ui
{
    NotepadWidget::NotepadWidget(const Notepad& notepad, const sf::Texture& background_texture,
        const sf::Font& font) : TexturedPanel(background_texture), notepad(notepad)
    {
        auto& players = notepad.get_players();
        const int alibi_length = notepad.get_notes(players[0]).size();
        float button_width = get_local_size().x / (players.size() + 1);
        note_panels.reserve(players.size());
        for(int i = 0; i < players.size(); ++i)
        {
            auto button = add_widget(
                Button()
                    .set_callback(std::bind(&NotepadWidget::switch_note, this, i))
                    .set_hover_color(Color(0, 0, 0, 100))
                    .set_active_color(Color(0, 0, 0, 200))
                    .set_position({(i + 1) * button_width, 50})
                    .set_size({button_width, 40})
                    .set_origin(Origin::CenterTop)
                    .set_anchor(Anchor::TopLeft)
            );
            button->add_widget(Text(font, players[i].get_nickname()));

            auto note_panel = (Panel*) add_widget(
                Panel()
                    .set_position({0, -30})
                    .set_size({0.9, 0.9}, true)
                    .set_enabled(false)
            );

            auto table_widget = (TableWidget*) note_panel->add_widget(
                TableWidget(3, alibi_length + 1)
                    .set_column_widths({150, 300, 100})
                    .set_row_heights(std::vector<float>(alibi_length + 1, 45))
                    .set_position({0, 150})
                    .set_origin(Origin::CenterTop)
                    .set_anchor(Origin::CenterTop)
            );
            table_widget->add_widget(
                0, 0, Text(font, "Time")
                    .set_font_size(28)
            );
            table_widget->add_widget(
                1, 0, Text(font, "Room")
                    .set_font_size(28)
            );
            table_widget->add_widget(
                2, 0, Text(font, "Sure")
                    .set_font_size(28)
            );

            auto& notes = notepad.get_notes(players[i]);
            for(int j = 0; j < notes.size(); ++j)
            {
                table_widget->add_widget(0, j + 1, Text(font, hours[j]));
                table_widget->add_widget(
                    1, j + 1, LineEdit(font, notes[j].room_name)
                        .set_background_color(Color::Transparent)
                        .set_border_color(Color::Transparent)
                        .set_text_color(Color::Black)
                        .set_max_length(24)
                        .set_size({290, 40})
                );
                table_widget->add_widget(
                    2, j + 1, Checkbox()
                        .set_checked(!notes[j].room_name.empty())
                        .set_background_color(Color::Transparent)
                        .set_outline_color(Color::Black)
                        .set_tick_color(Color::Black)
                        .set_size({30, 30})
                );
            }

            note_panels.push_back(note_panel);
        }

        note_panels[current_note]->set_enabled(true);
    }

    NotepadWidget::NotepadWidget(const NotepadWidget& other)
        : TexturedPanel(other), notepad(other.notepad), current_note(other.current_note)
    {
        auto& children = get_children();
        note_panels = get_children<Panel>();

        auto buttons = get_children<Button>();
        for(int i = 0; i < buttons.size(); ++i)
            buttons[i]->set_callback(std::bind(&NotepadWidget::switch_note, this, i));
    }

    Notepad& NotepadWidget::get_notepad()
    {
        return notepad;
    }

    const Notepad& NotepadWidget::get_notepad() const
    {
        return notepad;
    }

    void NotepadWidget::switch_note(int note)
    {
        note_panels[current_note]->set_enabled(false);
        current_note = note;
        note_panels[current_note]->set_enabled(true);
    }

    NotepadWidget* NotepadWidget::clone() const
    {
        return new NotepadWidget(*this);
    }
}

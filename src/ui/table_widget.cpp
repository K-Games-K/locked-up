#include "ui/panel.hpp"
#include "ui/table_widget.hpp"

namespace Ui
{
    TableWidget::TableWidget(int columns_count, int rows_count,
        const std::vector<float>& column_widths, const std::vector<float>& row_heights,
        sf::Vector2f position, TableWidgetSettings settings, Anchor origin, Anchor anchor)
        : Widget(position, {}, origin, anchor),
        columns_count(columns_count), rows_count(rows_count), column_widths(column_widths),
        row_heights(row_heights), settings(settings)
    {
        update_size();
    }

    TableWidget::~TableWidget()
    {
        for(auto& column : widgets)
            for(auto& widget : column)
                delete widget;
    }

    void TableWidget::handle_event(sf::Event event, sf::Vector2f mouse_pos)
    {
        if(!is_enabled())
            return;

        sf::Vector2f cell_pos;
        for(int column = 0; column < columns_count; ++column)
        {
            cell_pos.y = 0;
            for(int row = 0; row < rows_count; ++row)
            {
                auto& widget = widgets[column][row];
                if(widget == nullptr || !widget->is_enabled())
                    continue;

                sf::Vector2f cell_size(column_widths[column], row_heights[row]);
                widget->handle_event(
                    event, mouse_pos - widget->get_relative_position(cell_pos, cell_size)
                );
                cell_pos += sf::Vector2f(0, row_heights[row] + settings.grid_thickness);
            }
            cell_pos += sf::Vector2f(column_widths[column] + settings.grid_thickness, 0);
        }
    }

    void TableWidget::add_widget(int column, int row, Widget* widget)
    {
        if(dynamic_cast<Ui::Panel*>(widget))
            throw std::invalid_argument("Cannot insert Panel inside TableWidget!");

        auto& existing = widgets.at(column).at(row);
        if(existing != nullptr)
            delete existing;

        widgets[column][row] = widget;
    }

    int TableWidget::get_rows_count() const
    {
        return rows_count;
    }

    int TableWidget::get_columns_count() const
    {
        return columns_count;
    }

    const std::vector<std::vector<Widget*>>& TableWidget::get_widgets() const
    {
        return widgets;
    }

    void TableWidget::set_grid_color(sf::Color grid_color)
    {
        settings.grid_color = grid_color;
    }

    void TableWidget::set_grid_thickness(float grid_thickness)
    {
        settings.grid_thickness = grid_thickness;
    }

    TableWidgetSettings TableWidget::get_settings() const
    {
        return settings;
    }

    const std::vector<float>& TableWidget::get_column_widths() const
    {
        return column_widths;
    }

    const std::vector<float>& TableWidget::get_row_heights() const
    {
        return row_heights;
    }

    void TableWidget::update_size()
    {
        widgets.resize(columns_count);
        for(auto& column : widgets)
            column.resize(rows_count, nullptr);

        sf::Vector2f size;
        for(int i = 0; i < columns_count; ++i)
            size += {(float)column_widths[i], 0};
        for(int i = 0; i < rows_count; ++i)
            size += {0, (float)row_heights[i]};
        size += sf::Vector2f(columns_count - 1, rows_count - 1) * settings.grid_thickness;
        set_size(size);
    }
}

#include "render/table_widget_renderer.hpp"

TableWidgetRenderer::TableWidgetRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetContainerRenderer(window, resources)
{}

void TableWidgetRenderer::render(const Ui::TableWidget& table_widget, const float dt)
{
    sf::Vector2f table_pos = table_widget.get_relative_position(origin_pos, parent_size);
    Ui::TableWidgetSettings table_widget_settings = table_widget.get_settings();
    float table_width = table_widget.get_size().x;
    float table_height = table_widget.get_size().y;
    sf::RectangleShape grid_rect_vert({table_widget_settings.grid_thickness, table_height});
    grid_rect_vert.setFillColor(table_widget_settings.grid_color);
    sf::RectangleShape grid_rect_hori({table_width, table_widget_settings.grid_thickness});
    grid_rect_hori.setFillColor(table_widget_settings.grid_color);

    auto& column_widths = table_widget.get_column_widths();
    sf::Vector2f line_pos = table_pos;
    for(int i = 0; i < table_widget.get_columns_count() - 1; ++i)
    {
        line_pos += sf::Vector2f(column_widths[i] + table_widget_settings.grid_thickness, 0);
        grid_rect_vert.setPosition(line_pos);
        window.draw(grid_rect_vert);
    }

    auto& row_heights = table_widget.get_row_heights();
    line_pos = table_pos;
    for(int i = 0; i < table_widget.get_rows_count() - 1; ++i)
    {
        line_pos += sf::Vector2f(0, row_heights[i] + table_widget_settings.grid_thickness);
        grid_rect_hori.setPosition(line_pos);
        window.draw(grid_rect_hori);
    }

    auto& widgets = table_widget.get_widgets();
    sf::Vector2f cell_pos = table_pos;
    for(int column = 0; column < table_widget.get_columns_count(); ++column)
    {
        cell_pos.y = table_pos.y;
        for(int row = 0; row < table_widget.get_rows_count(); ++row)
        {
            auto& widget = widgets[column][row];
            sf::Vector2f cell_size(column_widths[column], row_heights[row]);

            WidgetContainerRenderer::render(widget, dt, cell_pos, cell_size);

            cell_pos += sf::Vector2f(0, row_heights[row] + table_widget_settings.grid_thickness);
        }
        cell_pos += sf::Vector2f(column_widths[column] + table_widget_settings.grid_thickness, 0);
    }
}

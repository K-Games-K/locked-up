#include "render/table_widget_renderer.hpp"

TableWidgetRenderer::TableWidgetRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window) {}

void TableWidgetRenderer::render(const Ui::TableWidget& table_widget, const float dt,
    sf::Vector2f parent_pos, sf::Vector2f parent_size)
{
    sf::Vector2f table_pos = table_widget.get_global_position(parent_pos, parent_size);
    float table_width = table_widget.get_global_size(parent_size).x;
    float table_height = table_widget.get_global_size(parent_size).y;
    sf::RectangleShape grid_rect_vert({table_widget.get_grid_thickness(), table_height});
    grid_rect_vert.setFillColor(table_widget.get_grid_color());
    sf::RectangleShape grid_rect_hori({table_width, table_widget.get_grid_thickness()});
    grid_rect_hori.setFillColor(table_widget.get_grid_color());

    auto& column_widths = table_widget.get_column_widths();
    sf::Vector2f line_pos = table_pos;
    for(int i = 0; i < table_widget.get_columns_count() - 1; ++i)
    {
        line_pos += sf::Vector2f(column_widths[i] + table_widget.get_grid_thickness(), 0);
        grid_rect_vert.setPosition(line_pos);
        window.draw(grid_rect_vert);
    }

    auto& row_heights = table_widget.get_row_heights();
    line_pos = table_pos;
    for(int i = 0; i < table_widget.get_rows_count() - 1; ++i)
    {
        line_pos += sf::Vector2f(0, row_heights[i] + table_widget.get_grid_thickness());
        grid_rect_hori.setPosition(line_pos);
        window.draw(grid_rect_hori);
    }
}

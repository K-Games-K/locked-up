#include "table_widget.hpp"

namespace Ui
{
    TableWidget::TableWidget(int columns_count, int rows_count)
        : columns_count(columns_count), rows_count(rows_count),
        column_widths(std::vector<float>(columns_count)),
        row_heights(std::vector<float>(rows_count))
    {
        update_size();
    }

    Widget* TableWidget::add_widget(unsigned int column, unsigned int row, const Widget& widget)
    {
        if(column < columns_count && row < rows_count)
            return get_children().at(column + row * columns_count)->add_widget(widget);

        return nullptr;
    }

    int TableWidget::get_rows_count() const
    {
        return rows_count;
    }

    int TableWidget::get_columns_count() const
    {
        return columns_count;
    }

    TableWidget& TableWidget::set_grid_color(Color grid_color)
    {
        this->grid_color = grid_color;

        return *this;
    }

    Color TableWidget::get_grid_color() const
    {
        return grid_color;
    }

    TableWidget& TableWidget::set_grid_thickness(float grid_thickness)
    {
        this->grid_thickness = grid_thickness;
        update_size();

        return *this;
    }

    float TableWidget::get_grid_thickness() const
    {
        return grid_thickness;
    }

    TableWidget& TableWidget::set_column_widths(const std::vector<float>& column_widths)
    {
        this->column_widths = column_widths;
        update_size();

        return *this;
    }

    const std::vector<float>& TableWidget::get_column_widths() const
    {
        return column_widths;
    }

    TableWidget& TableWidget::set_row_heights(const std::vector<float>& row_heights)
    {
        this->row_heights = row_heights;
        update_size();

        return *this;
    }

    const std::vector<float>& TableWidget::get_row_heights() const
    {
        return row_heights;
    }

    void TableWidget::update_size()
    {
        // Resize children instead of recreating them.
        get_children().clear();
        sf::Vector2f cell_pos = {0, 0};
        for(int row = 0; row < rows_count; ++row)
        {
            cell_pos.x = 0;
            for(int column = 0; column < columns_count; ++column)
            {
                Widget::add_widget(
                    Item()
                        .set_position(cell_pos)
                        .set_size({column_widths[column], row_heights[row]})
                        .set_origin(Origin::TopLeft)
                        .set_anchor(Anchor::TopLeft)
                );
                cell_pos += sf::Vector2f(column_widths[column] + grid_thickness, 0);
            }
            cell_pos += sf::Vector2f(0, row_heights[row] + grid_thickness);
        }

        sf::Vector2f size = {0, 0};
        for(float column_width : column_widths)
            size += {column_width, 0};
        for(float row_height : row_heights)
            size += {0, row_height};
        size += sf::Vector2f(columns_count - 1, rows_count - 1) * this->grid_thickness;
        set_size(size);
    }

    TableWidget* TableWidget::clone() const
    {
        return new TableWidget(*this);
    }

    TableWidget::Item* TableWidget::Item::clone() const
    {
        return new Item(*this);
    }
}

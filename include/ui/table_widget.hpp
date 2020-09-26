#pragma once

#include "widget.hpp"

namespace Ui
{
    class TableWidget : public Widget
    {
    public:
        TableWidget(int columns_count, int rows_count);

        Widget* add_widget(unsigned int column, unsigned int row, const Widget& widget);

        int get_rows_count() const;

        int get_columns_count() const;

        TableWidget& set_grid_color(Color grid_color);

        Color get_grid_color() const;

        TableWidget& set_grid_thickness(float grid_thickness);

        float get_grid_thickness() const;

        TableWidget& set_column_widths(const std::vector<float>& column_widths);

        const std::vector<float>& get_column_widths() const;

        TableWidget& set_row_heights(const std::vector<float>& row_heights);

        const std::vector<float>& get_row_heights() const;

    private:
        class Item : public Widget
        {
        private:
            Item* clone() const override;
        };

        const int columns_count;
        const int rows_count;

        Color grid_color = Color::Black;
        float grid_thickness = 1;

        std::vector<float> column_widths;
        std::vector<float> row_heights;

        void update_size();

        TableWidget* clone() const override;
    };
}

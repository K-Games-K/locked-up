#pragma once

#include "widget_container.hpp"

namespace Ui
{
    struct TableWidgetSettings
    {
        sf::Color grid_color = sf::Color::Black;
        float grid_thickness = 1;
    };

    class TableWidget : public WidgetContainer
    {
    private:
        const int columns_count;
        const int rows_count;

        TableWidgetSettings settings;

        std::vector<std::vector<Widget*>> widgets;
        std::vector<float> column_widths;
        std::vector<float> row_heights;

        void update_size();

    public:
        TableWidget(int columns_count = 0, int rows_count = 0,
            const std::vector<float>& column_widths = {},
            const std::vector<float>& row_heights = {},
            sf::Vector2f position = {0, 0}, TableWidgetSettings settings = TableWidgetSettings(),
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        ~TableWidget();

        void handle_event(sf::Event event, sf::Vector2f mouse_pos) override;

        void update(const float dt) override;

        void add_widget(int column, int row, Widget* widget);

        int get_rows_count() const;

        int get_columns_count() const;

        void set_grid_color(sf::Color grid_color);

        void set_grid_thickness(float grid_thickness);

        TableWidgetSettings get_settings() const;

        const std::vector<float>& get_column_widths() const;

        const std::vector<float>& get_row_heights() const;

        const std::vector<std::vector<Widget*>>& get_widgets() const;
    };
}

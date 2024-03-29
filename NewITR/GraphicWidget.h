#pragma once

#include <QWidget>

class GraphicWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GraphicWidget(QWidget* parent = nullptr);

    // EVENTS
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

	/// \brief Добавляет фигуру на отрисовку
	void AddFigure();

private:
    // UI VIEW PARAMS
    int scale_;
    int point_radius_;
    QPoint center_point_;
    QPoint drag_start_;
    bool is_dragging_;

    // UI VIEW METHODS
    void DrawFigure(QPainter& painter);
    void DrawScale(QPainter& painter, int width, int height) const;
    void DrawAxisLabels(QPainter& painter, int width, int height) const;
    void DrawGrid(QPainter& painter, int width, int height) const;
};


#include "GraphicWidget.h"

#include <QtWidgets>

GraphicWidget::GraphicWidget(QWidget* parent) : QWidget(parent)
{
    scale_ = 20;
    point_radius_ = 3;
    is_dragging_ = false;
    setMouseTracking(true);
    center_point_ = rect().center();
}

// ===================== Draw Methods =====================

void GraphicWidget::DrawFigure(QPainter& painter)
{
    /*painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    QPolygonF triangle;
    QPointF p;
    for (int i = 0; i < points.length(); i += 2)
    {
        p.setX(center_point_.x() + points[i] * scale_);
        p.setY(center_point_.y() - points[i + 1] * scale_);
        triangle << p;
    }
    painter.drawConvexPolygon(triangle);
    triangle.clear();*/
}

void GraphicWidget::DrawScale(QPainter& painter, const int width, const int height) const
{
    const int step = scale_;

    for (int x = center_point_.x() + scale_ * step; x < width; x += scale_ * step)
        painter.drawLine(x, center_point_.y() - 3, x, center_point_.y() + 3);
    for (int x = center_point_.x() - scale_ * step; x > 0; x -= scale_ * step)
        painter.drawLine(x, center_point_.y() - 3, x, center_point_.y() + 3);

    for (int y = center_point_.y() + scale_ * step; y < height; y += scale_ * step)
        painter.drawLine(center_point_.x() - 3, y, center_point_.x() + 3, y);
    for (int y = center_point_.y() - scale_ * step; y > 0; y -= scale_ * step)
        painter.drawLine(center_point_.x() - 3, y, center_point_.x() + 3, y);
}

void GraphicWidget::DrawAxisLabels(QPainter& painter, const int width, const int height) const
{
	const QFontMetrics metrics(painter.font());
	const int text_height = metrics.height();

    for (int x = center_point_.x() + scale_; x < width; x += scale_) {
        QString label = QString::number((x - center_point_.x()) / scale_);
        QRect bounding_rect = metrics.boundingRect(label);
        const int label_width = bounding_rect.width();
        painter.drawText(x - label_width / 2, center_point_.y() + text_height + 10, label);
    }
    for (int x = center_point_.x() - scale_; x > 0; x -= scale_) {
        QString label = QString::number((x - center_point_.x()) / scale_);
        QRect bounding_rect = metrics.boundingRect(label);
        const int label_width = bounding_rect.width();
        painter.drawText(x - label_width / 2, center_point_.y() + text_height + 10, label);
    }

    for (int y = center_point_.y() + scale_; y < height; y += scale_) {
        QString label = QString::number((center_point_.y() - y) / scale_);
        QRect bounding_rect = metrics.boundingRect(label);
        const int label_width = bounding_rect.width();
        painter.drawText(center_point_.x() - label_width - 10, y + text_height / 2, label);
    }
    for (int y = center_point_.y() - scale_; y > 0; y -= scale_) {
        QString label = QString::number((center_point_.y() - y) / scale_);
        QRect bounding_rect = metrics.boundingRect(label);
        const int label_width = bounding_rect.width();
        painter.drawText(center_point_.x() - label_width - 10, y + text_height / 2, label);
    }

	const QRect x_axis_label_rect = metrics.boundingRect("X");
    painter.drawText(width - x_axis_label_rect.width() - 3, center_point_.y() + text_height, "X");
    painter.drawText(center_point_.x() + 5, text_height, "Y");
}

void GraphicWidget::DrawGrid(QPainter& painter, const int width, const int height) const
{
    const int cell_size = scale_;

    const QPen grid_pen(Qt::lightGray, 1, Qt::DashLine);
    painter.setPen(grid_pen);

    for (int x = center_point_.x() + cell_size; x < width; x += cell_size)
        painter.drawLine(x, 0, x, height);
    for (int x = center_point_.x() - cell_size; x > 0; x -= cell_size)
        painter.drawLine(x, 0, x, height);

    for (int y = center_point_.y() + cell_size; y < height; y += cell_size)
        painter.drawLine(0, y, width, y);
    for (int y = center_point_.y() - cell_size; y > 0; y -= cell_size)
        painter.drawLine(0, y, width, y);
}

// ===================== Draw Methods END =====================

// ===================== Events =====================

void GraphicWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = size().width();
    int height = size().height();

    painter.drawLine(0, center_point_.y(), width, center_point_.y()); // Îñü X
    painter.drawLine(center_point_.x(), 0, center_point_.x(), height); // Îñü Y

    DrawScale(painter, width, height);
    DrawAxisLabels(painter, width, height);
    DrawGrid(painter, width, height);
    DrawFigure(painter);

    Q_UNUSED(event)
}

void GraphicWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        drag_start_ = event->pos();
        is_dragging_ = true;
    }
}

void GraphicWidget::mouseMoveEvent(QMouseEvent* event) {
    if (is_dragging_) {
	    const QPoint delta = event->pos() - drag_start_;
        center_point_ -= delta;
        drag_start_ = event->pos();
        update();
    }
}

void GraphicWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        is_dragging_ = false;
    }
}

void GraphicWidget::wheelEvent(QWheelEvent* event) {
	if (const QPoint num_degrees = event->angleDelta() / 8; !num_degrees.isNull()) {
	    const QPoint num_steps = num_degrees / 15;
        scale_ += num_steps.y();
        if (scale_ < 1)
            scale_ = 1;
        update();
    }
    event->accept();
}

// ===================== Events END =====================
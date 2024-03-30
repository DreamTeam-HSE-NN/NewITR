#pragma once

#include <QWidget>

#include "Types.h"

class GraphicWidget : public QWidget
{
	Q_OBJECT

public:
	GraphicWidget(QWidget* parent = nullptr);

	/// \brief Добавляет фигуры на отрисовку
	void SetFigures(CalcHelperRetVal param);

private:
	/// \brief Склад фигур для отрисовки
	QList<UiFigureEntity> figures_;

    // UI VIEW PARAMS
    int scale_;
    QPoint center_point_;
    QPoint drag_start_;
    bool is_dragging_;

    // UI VIEW METHODS
    void DrawFigures(QPainter& painter);
    void DrawScale(QPainter& painter, int width, int height) const;
    void DrawAxisLabels(QPainter& painter, int width, int height) const;
    void DrawGrid(QPainter& painter, int width, int height) const;

    // EVENTS
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    // костыль
    bool on_init_ = true;

signals:
    void SendMsgToStatusBar(QString msg);
};


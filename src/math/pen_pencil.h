#ifndef PEN_PENCIL_H
#define PEN_PENCIL_H
#include "bezier.h"
#include <QPixmap>

#include <QPainter>
#include <interfaces.h>

class PencilStyle :public QObject,public BrushInterface{
	Q_OBJECT
public:
    explicit PencilStyle(QObject *parent=nullptr);

	~PencilStyle() override;

private:
	void insert_first(float x, float y);
	void insert(float x, float y);
	void insert_last(float x, float y);


	spoint get_last_inserted() {
		spoint p = { 0, 0 };
		if (m_cur_path) {
			p = m_cur_path->last_point;
		} 
		return p;
	}

public:


	QRect mousePress(const QString &brush, QImage *image, const QPointF &pos) override;

	QRect mouseMove(const QString &brush, QImage *image, const QPointF &newPos) override;

	QRect mouseRelease(const QString &brush, QImage *image, const QPointF &pos) override;

    QRect drawInternal(QPaintDevice *device) override;

private:
	wpoint_arraylist *m_arr_list = NULL;
	wpoints_array *m_cur_path = NULL;
	unsigned int m_cur_last_index = 0;
	float m_w_max, m_w_min; 
    QPixmap *m_mem_pixmap{nullptr};
	unsigned int m_cx = 0, m_cy = 0;




};

#endif //PEN_PELCIL_H

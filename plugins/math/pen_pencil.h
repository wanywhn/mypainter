#ifndef PEN_PENCIL_H
#define PEN_PENCIL_H
#include "bezier.h"
#include <QPixmap>

#include <QPainter>
#include <interfaces.h>

class PenCommonCommand:public CommandInterface{

public:
    PenCommonCommand(DrawPathParameter parameter);

    // CommandInterface interface
public:
    QRect undo() Q_DECL_OVERRIDE;
    QRect redo() Q_DECL_OVERRIDE;

private:
    DrawPathParameter m_parameter;
};

class PencilStyle :public QObject,public BrushInterface{
	Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PlugAndPaint.BrushInterface" FILE "basictools.json")
	Q_INTERFACES(BrushInterface)
public:
    explicit PencilStyle(QObject *parent=nullptr);

	~PencilStyle() override;

private:
	void insert_first(int x, int y);
	void insert(int x, int y);
	void insert_last(int x, int y);


	spoint get_last_inserted() {
		spoint p = { 0, 0 };
		if (m_cur_path) {
			p = m_cur_path->last_point;
		} 
		return p;
	}

public:

	QStringList brushes() const override;

	QRect mousePress(const QString &brush, QPainter &painter, const QPoint &pos) override;

	QRect mouseMove(const QString &brush, QPainter &painter, const QPoint &newPos) override;

	QRect mouseRelease(const QString &brush, QPainter &painter, const QPoint &pos) override;

    QRect drawInternal(QPainterPath *ppath) override;

private:
	wpoint_arraylist *m_arr_list = NULL;
	wpoints_array *m_cur_path = NULL;
	unsigned int m_cur_last_index = 0;
	float m_w_max, m_w_min; 
    QPixmap *m_mem_pixmap{nullptr};
	unsigned int m_cx = 0, m_cy = 0;



public:
    CommandInterface *createCommand(DrawPathParameter parameter) Q_DECL_OVERRIDE;

    // BrushInterface interface
public:
    void draw(QPainter *painter, DrawPathParameter drawPathObj) Q_DECL_OVERRIDE;

};

#endif //PEN_PELCIL_H

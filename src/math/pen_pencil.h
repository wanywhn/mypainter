#ifndef PEN_PENCIL_H
#define PEN_PENCIL_H
#include "bezier.h"

#include <QPixmap>


class PencilStyle :public QObject{
	Q_OBJECT
public:
	PencilStyle(float min_pen_width, float max_pen_width);
	~PencilStyle() override;

	void init_canvas(unsigned int cx, unsigned int cy);
    QPixmap &getPixmap();

	void insert_first(int x, int y);
	void insert(int x, int y);
	void insert_last(int x, int y);

	void resize(int x,int y);

	spoint get_last_inserted() {
		spoint p = { 0, 0 };
		if (m_cur_path) {
			p = m_cur_path->last_point;
		} 
		return p;
	}

    bool draw_internal(QRegion &region);
private:
	wpoint_arraylist *m_arr_list = NULL;
	wpoints_array *m_cur_path = NULL;
	unsigned int m_cur_last_index = 0;
	float m_w_max, m_w_min; 
    QPixmap *m_mem_pixmap{nullptr};
	unsigned int m_cx = 0, m_cy = 0;
};

#endif //PEN_PELCIL_H

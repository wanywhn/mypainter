#include "pen_pencil.h"

#include <QPainter>
#include <stdio.h>
#include <QDebug>

PencilStyle::PencilStyle(float min_pen_width, float max_pen_width):
	QObject(nullptr),m_w_min(min_pen_width), m_w_max(max_pen_width)
{
	m_arr_list = z_new_fpoint_arraylist();

}

PencilStyle::~PencilStyle()
{ 

    if(m_mem_pixmap) delete m_mem_pixmap;
	if (m_cur_path) z_drop_fpoint_array(m_cur_path);
	if (m_arr_list) z_drop_fpoint_arraylist(m_arr_list);
}

void PencilStyle::init_canvas(unsigned int cx, unsigned int cy)
{ 
    this->m_mem_pixmap=new QPixmap(cx,cy);
    this->m_mem_pixmap->fill();
}



QPixmap &PencilStyle::getPixmap()
{
    return *m_mem_pixmap;
}

void PencilStyle::insert_first(int x, int y)
{
	spoint point = { (float)x, (float)y };
	if (m_cur_path) 
		z_drop_fpoint_array(m_cur_path); 

	m_cur_path = z_fpoint_arraylist_append_new(m_arr_list, m_w_max, m_w_min);

	z_insert_point(m_cur_path, point); 
}

void PencilStyle::insert(int x, int y)
{ 
	spoint point = { (float)x, (float)y };
	if(!m_cur_path) 
		m_cur_path = z_fpoint_arraylist_append_new(m_arr_list, m_w_max, m_w_min);

	z_insert_point(m_cur_path, point);
}

void PencilStyle::insert_last(int x, int y)
{
	if (!m_cur_path)
		return;
	spoint point = { (float)x, (float)y };
	z_insert_last_point(m_cur_path, point);

	z_drop_fpoint_array(m_cur_path);
    m_cur_path = nullptr;
}

bool PencilStyle::draw_internal(QRegion &region)
{

	if (!m_arr_list || !m_arr_list->first) return false;

	wpoint_arraylist_node *node = m_arr_list->cur;


	QRegion updateed{0,0,0,0};

    QPainter painter(m_mem_pixmap);
//    QPainter painter(m_mem_pixmap);
//    painter.setRenderHint();

    auto pen=painter.pen();
    pen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pen.setColor(Qt::red);
    pen.setCapStyle(Qt::RoundCap);

	int l, t, r, b;
	float fx, fy, tx, ty, fw, tw;
	wpoints_array *path = NULL;
	wfpoint *p = NULL, *last = NULL;

	while (node) {
		m_arr_list->cur = node;

		path = node->a;
		//一个path就是一个连笔
		p = path->point + m_cur_last_index;
		last = path->point + (path->len - 1);


		while ( p <last ) {
			float fontwid=p->w*m_w_max;
            pen.setWidthF(fontwid);

            fx = p->p.x; fy = p->p.y; fw = p->w;  p++;
            tx = p->p.x; ty = p->p.y; tw = p->w;

            //FIXME 有跳跃？
            static QLineF line;
            line.setLine(fx,fy,tx,ty);
            painter.setPen(pen);
            painter.drawLine(line);

			if (fx < tx) {
				l = fx; r = tx;
			}
			else {
				r = fx; l = tx;
			}

			if (fy < ty) {
				t = fy; b = ty;
			}
			else {
				b = fy; t = ty;
			}
			updateed=updateed.united(QRect(l-fontwid,t-fontwid,r+fontwid,b+fontwid));
		};

		node = node->n;
		m_cur_last_index = 0;
	}; 

	m_arr_list->cur = m_arr_list->end;

	region=region.united(updateed);

    return true;


}

void PencilStyle::resize(int x, int y) {
    if(m_mem_pixmap){
    	delete m_mem_pixmap;
    }
    m_mem_pixmap=new QPixmap(x,y);
	this->m_mem_pixmap->fill();

}


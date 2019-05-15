#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "bezier.h"

#define min(X, Y) ((X) < (Y) ? (X) : (Y))

#define z_malloc_struct(t) (t*)calloc(1, sizeof(t))

static void *z_malloc_array(unsigned int count, unsigned int size);

static void *z_resize_array(void *p, size_t count, size_t size);

static void z_fpoint_array_set_last_info(wpoints_array *arr, spoint last_point, float last_width);

static float z_square(float f) { return (float) f * f; };

static float z_cubic_(float f) { return (float) powf(f, 3); };

typedef struct z_bezier_factors_s {
    float bezier_step;      // must be divisible by 1.0f
    float max_width_diff;   // max width diff between two near lines
    float max_move_speed;   // 
    float max_linewith;
} z_bezier_factors;

int z_point_equals(spoint *p1, spoint *p2) {
    return (p1->x == p2->x && p1->y == p2->y) ? 1 : 0;
}


static const float defualt_max_width = 100.0f;
static const float default_min_width = 5.0f;


float z_movespeed(tpoint s, tpoint e) {
    float d = z_distance(s.p, e.p);
    return (0 == d) ? 0 : d / (e.t - s.t);
}

float z_distance(spoint b, spoint e) {
    return (float) sqrtf(z_square(e.x - b.x) + z_square(e.y - b.y));
}




z_list *z_list_new(z_list_node_alloc_fun allocfun, z_list_node_drop_fun dropfun) {
    z_list *l = NULL;
    l = z_malloc_struct(z_list);
    l->alloc = allocfun;
    l->drop = dropfun;
    l->first = l->last = NULL;
    return l;
}

void *z_list_append_new(z_list *zlist) {
    z_list_node *node = NULL;
    void *data = NULL;

    if (!zlist->alloc || !zlist->drop)
        return NULL;

    node = z_malloc_struct(z_list_node);
    node->data = zlist->alloc();
    node->n = NULL;
    node->p = NULL;

    if (node) {
        if (!zlist->first) {
            zlist->first = zlist->last = node;
        } else {
            node->n = NULL;
            node->p = zlist->last;
            zlist->last->n = node;
            zlist->last = node;
        }
        data = node->data;
    }

    return data;
}

void *z_list_remove_last(z_list *zlist) {
    void *data = NULL;
    z_list_node *tmp = zlist->last;
    if (zlist->last) {
        tmp = zlist->last;
        if (zlist->last == zlist->first) {
            zlist->last = zlist->first = NULL;
        } else {
            zlist->last = tmp->p;
            zlist->last->n = NULL;
        }
    }

    if (tmp) {
        data = tmp->data;
        free(tmp);
    }

    return data;
}

void z_list_clear(z_list *zlist) {
    while (zlist->first)
        zlist->drop(z_list_remove_last(zlist));
}

void z_list_free(z_list *zlist) {
    z_list_clear(zlist);
    free(zlist);
}


void *z_malloc_array(unsigned int count, unsigned int size) {
    unsigned int totalsize = count * size;
    if (totalsize <= 0) return 0;

    void *buffer = malloc(count * size);
    if (buffer) memset(buffer, 0, count * size);
    return buffer;
}

void *z_resize_array(void *p, size_t count, size_t size) {
    void *np = 0;
    size_t total_size = count * size;

    if (total_size <= 0)
        return np;

    np = realloc(p, total_size);

    return np;
}

void z_fpoint_array_set_last_info(wpoints_array *arr, spoint last_point, float last_width) {
    if (!arr) return;
    arr->last_point = last_point;
    arr->last_ms = clock() * (1000.0f / CLOCKS_PER_SEC);
    arr->last_width = last_width;
//	printf("reset last ms to 0x%li\n", arr->last_ms);
}

void BezierBase::insert_first(float x, float y) {
    spoint point = {x, y};
    if (m_cur_path)
        z_drop_fpoint_array(m_cur_path);

    m_cur_path = z_fpoint_arraylist_append_new(m_arr_list);

    z_insert_point(m_cur_path, point);

}

void BezierBase::insert(float x, float y) {
    spoint point = {(float) x, (float) y};
    if (!m_cur_path)
        m_cur_path = z_fpoint_arraylist_append_new(m_arr_list);
    z_insert_point(m_cur_path, point);
}

void BezierBase::insert_last(float x, float y) {
    if (!m_cur_path)
        return;
    spoint point = {x, y};
    z_insert_last_point(m_cur_path, point);

    z_drop_fpoint_array(m_cur_path);
    m_cur_path = nullptr;
}

BezierBase::~BezierBase() {
    if (m_cur_path) z_drop_fpoint_array(m_cur_path);
    if (m_arr_list) z_drop_fpoint_arraylist(m_arr_list);
}

wpoints_array *BezierBase::z_fpoint_arraylist_append_new(wpoint_arraylist *l) {
    wpoints_array *a = z_new_fpoint_array(24);
    z_fpoint_arraylist_append(l, a);
    return a;
}

void BezierBase::z_drop_fpoint_arraylist(wpoint_arraylist *l) {

    if (!l) return;

    if (!(--(l->ref))) {
        wpoint_arraylist_node *c = l->first;
        wpoint_arraylist_node *n;
        while (c) {
            z_drop_fpoint_array(c->a);
            n = c->n;
            free(c);
            c = n;
        }
    }

}

void BezierBase::z_drop_fpoint_array(wpoints_array *a) {
    if (!a) return;
    if (!(--(a->ref))) {
        free(a);
    }


}

void BezierBase::z_fpoint_arraylist_append(wpoint_arraylist *l, wpoints_array *a) {

    wpoint_arraylist_node *node = z_malloc_struct(wpoint_arraylist_node);
    node->a = z_keep_fpoint_array(a);
    node->n = NULL;

    if (!l->first) {
        l->first = node;
    } else {
        l->end->n = node;
    }

    l->end = node;

}

wpoints_array *BezierBase::z_keep_fpoint_array(wpoints_array *a) {

    if (a) a->ref++;
    return a;
}

void BezierBase::z_insert_last_point(wpoints_array *arr, spoint e) {


    if (!arr) return;
    long len = arr->len;
    if (len == 0) return;
    wpoints_array *points = z_new_fpoint_array(51);
    wfpoint zb = arr->point[len - 1];
    z_fpoint_add(points, zb);

    wfpoint ze = {{e.x, e.y}, 0.1f};
    z_fpoint_differential_add(points, ze);
    int i;
    for (i = 1; i < points->len; i++) {
        z_fpoint_add(arr, points->point[i]);
    }
    z_drop_fpoint_array(points);

}

float BezierBase::z_insert_point(wpoints_array *arr, spoint new_point) {


    if (!arr) return 0;
    int len = arr->len;

    if (0 == len) {
        //TODO 首点
        wfpoint p = {new_point,m_width};
        z_fpoint_add(arr, p);
        z_fpoint_array_set_last_info(arr, new_point, p.w);
        return p.w;
    }

    float cur_ms = clock() * (1000.0f / CLOCKS_PER_SEC);
    float last_width = arr->last_width;
    float last_ms = arr->last_ms;
    spoint last_point = arr->last_point;

//    printf("cur_ms - last_ms = %f\n", cur_ms - last_ms);
//TODO 启动伐值,控制连贯性
    float distance = z_distance(new_point, last_point);
    if ((cur_ms - last_ms) < 1 || distance < 2) {
        return 0;
    }

    float step = arr->len > 4 ? 0.4f : 0.8f;
    tpoint bt = {{last_point.x, last_point.y}, last_ms};
    tpoint et = {new_point, cur_ms};
    float w = (z_linewidth(bt, et, last_width, step) + last_width) / 2;
    wpoints_array *points = z_new_fpoint_array(51);
    wfpoint generated_lastpoint = arr->point[len - 1];
    z_fpoint_add(points, generated_lastpoint);


    //TODO k=2
    float k1=2.1;
    ///这一点为第二点时。直接按比例生成一个中间点。k>1有回峰效果
    if (1 == len) {
        wfpoint p = {getPercentPoint(bt.p,et.p,k1), w};
        z_fpoint_differential_add(points, p);
        w = p.w;
    } else {
        ///
        float k2=0.5;
        wfpoint bw = generated_lastpoint;
        spoint c = {last_point.x, last_point.y};
//        wfpoint ew = {{(last_point.x + new_point.x) * k2, (last_point.y + new_point.y) * k2}, w};
        wfpoint ew = {getPercentPoint(last_point,new_point,k2), w};
        z_square_bezier(points, bw, c, ew);
    }

    // escape the first spoint
    int i;
    for (i = 1; i < points->len; i++) {
        z_fpoint_add(arr, points->point[i]);
    }

    z_drop_fpoint_array(points);
    z_fpoint_array_set_last_info(arr, new_point, w);

    return w;
}

void BezierBase::z_fpoint_arraylist_removelast(wpoint_arraylist *l) {
    wpoint_arraylist_node *c = l->first;

    z_drop_fpoint_array(l->end->a);
    free(l->end);

    while (c->n != l->end) { c = c->n; }

    c->n = nullptr;
    l->end = c;

}

wpoint_arraylist *BezierBase::z_keep_fpoint_arraylist(wpoint_arraylist *l) {
    if (!l) return nullptr;
    l->ref++;
    return l;
}

wpoints_array *BezierBase::z_new_fpoint_array(int initsize) {
    if (initsize <= 0) return nullptr;
    wpoints_array *a = static_cast<wpoints_array *>(malloc(sizeof(wpoints_array)));
    a->point = static_cast<wfpoint *>(z_malloc_array(initsize, sizeof(wfpoint)));
    a->ref = 1;
    a->len = 0;

//    if (maxwidth < 0 || minwidth < 0 || maxwidth < minwidth) {
//        maxwidth = defualt_max_width;
//        minwidth = default_min_width;
//    }

//    a->maxwidth = maxwidth;
//    a->minwidth = minwidth;

    a->cap = initsize;
    return a;
}

wpoints_array *BezierBase::z_resize_fpoints_array(wpoints_array *a, int size) {
    if (!a || size <= 0) return NULL;

    a->point = (wfpoint *) z_resize_array(a->point, size, sizeof(wfpoint));
    a->cap = size;
    a->len = min(a->cap, a->len);
    return a;
}

wpoint_arraylist *BezierBase::z_new_fpoint_arraylist() {
    wpoint_arraylist *l = z_malloc_struct(wpoint_arraylist);
    l->ref = 1;
    l->first = l->end = NULL;
    return l;
}

BezierBase::BezierBase() {
    m_arr_list = z_new_fpoint_arraylist();
}

wpoint_arraylist *BezierBase::getM_arr_list() const {
    return m_arr_list;
}

float BezierBase::getM_w_max() const {
    return m_w_max;
}

void BezierBase::z_fpoint_add_xyw(wpoints_array *a, float x, float y, float w) {

    if (!a || (a->point[a->len - 1].p.x == x && a->point[a->len - 1].p.y == y)) return;

    if (a->len == a->cap)
        z_auto_increase_fpoints_array(a);

    wfpoint *p = a->point + (a->len++);
    p->p.x = x;
    p->p.y = y;
    p->w = w;
}

wpoints_array *BezierBase::z_auto_increase_fpoints_array(wpoints_array *a) {
    int cap = a->cap + (a->cap + 3) / 4;
    return z_resize_fpoints_array(a, cap);
}

void BezierBase::z_fpoint_add(wpoints_array *a, wfpoint p) {
    z_fpoint_add_xyw(a, p.p.x, p.p.y, p.w);

}

void BezierBase::z_fpoint_differential_add(wpoints_array *a, wfpoint p) {


    if (!a) return;

    if (a->len == 0) {
        z_fpoint_add(a, p);
        return;
    }

// #define bad_show
#ifdef bad_show
    z_fpoint_add(a, p);
return;
#endif
    float max_diff = 0.1f;
    wfpoint *last = a->point + (a->len - 1);
    spoint sp = last->p;
    float sw = last->w;

    int n = (int) ((fabsf(p.w - last->w) / max_diff) + 1);
    float x_step = (p.p.x - sp.x) / n;
    float y_step = (p.p.y - sp.y) / n;
    float w_step = (p.w - sw) / n;

    int i;
    for (i = 0; i < (n - 1); i++) {
        sp.x += x_step;
        sp.y += y_step;
        sw += w_step;
        z_fpoint_add_xyw(a, sp.x, sp.y, sw);
    }
    z_fpoint_add(a, p);
}

void BezierBase::z_square_bezier(wpoints_array *a, wfpoint b, spoint c, wfpoint e) {

    if (!a) return;
    const float f = 0.1f;
    for (float t = f; t <= 1.0; t += f) {
        float x1 = z_square(1 - t) * b.p.x + 2 * t * (1 - t) * c.x + z_square(t) * e.p.x;
        float y1 = z_square(1 - t) * b.p.y + 2 * t * (1 - t) * c.y + z_square(t) * e.p.y;
        float w = b.w + (t * (e.w - b.w));
        wfpoint pw = {{x1, y1}, w};
        z_fpoint_differential_add(a, pw);
    }
}

void BezierBase::setWidth(float width) {

    m_width=width;
    m_w_max = width * 1.4;
    m_w_min = width * 0.3;
}

spoint BezierBase::getPercentPoint(spoint p1, spoint p2, float percent) {
    spoint tmp;
    if(p1.x<p2.x){
        tmp.x=p1.x;
        tmp.x+=(p2.x-p1.x)*percent;
    }else{
        tmp.x=p2.x;
        tmp.x+=(p1.x-p2.x)*(1-percent);
    }
    if(p1.y<p2.y){
        tmp.y=p1.y;
        tmp.y+=(p2.y-p1.y)*percent;
    }else{
        tmp.y=p2.y;
        tmp.y+=(p1.y-p2.y)*(1-percent);
    }
    return tmp;
}

float BezierBase::z_linewidth(tpoint b, tpoint e, float bw, float step) {

        const float max_speed = 10.0f;
        float d = z_distance(b.p, e.p);
        //TODO 获取dpi
        float s = d / (e.t - b.t);
        s = s > max_speed ? max_speed : s;
        float w = ((max_speed - s) / max_speed)*m_width;
        float max_dif = d * step*m_width;
        if (w < m_w_min) w = m_w_min;
        if (fabs(w - bw) > max_dif) {
            if (w > bw)
                w = bw + max_dif;
            else
                w = bw - max_dif;
        }
        printf("d:%.4f, time_diff:%f, speed:%.4f, width:%.4f\n", d, e.t - b.t, s,w);
        return w;
}





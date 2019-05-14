#ifndef z_math_h_
#define z_math_h_

#include <stdint.h>

typedef struct spoint_s spoint;
typedef struct wpoint_s wfpoint;
typedef struct tpoint_s tpoint;
typedef struct wpoint_array_s wpoints_array;
typedef struct wpoint_arraylist_node_s wpoint_arraylist_node;
typedef struct wpoint_arraylist_s wpoint_arraylist;


struct spoint_s {
    float x, y;
};

struct wpoint_s {
    spoint p;
    float w;
};

struct tpoint_s {
    spoint p;
    float t;
};

struct wpoint_array_s {
    wfpoint *point;
    float maxwidth;
    float minwidth;
    int ref;
    int len;
    int cap;

    spoint last_point;
    float last_width;
    float last_ms;
};

struct wpoint_arraylist_node_s {
    wpoints_array *a;
    wpoint_arraylist_node *n;
};

struct wpoint_arraylist_s {
    int ref;
    wpoint_arraylist_node *first;
    wpoint_arraylist_node *end;
    wpoint_arraylist_node *cur;
};


class BezierBase {
public:

    BezierBase();

    void insert_first(float x, float y);

    void insert(float x, float y);

    void insert_last(float x, float y);

    virtual void setWidth(float width) {

        m_w_max = width * 1.5;
        m_w_min = width * 0.5;
    }

    virtual ~BezierBase();

protected:
    virtual void z_insert_last_point(wpoints_array *arr, spoint e);

    virtual float z_insert_point(wpoints_array *arr, spoint new_point);

    virtual void z_fpoint_differential_add(wpoints_array *a, wfpoint p);

private:

    void z_square_bezier(wpoints_array *a, wfpoint b, spoint c, wfpoint e);

    void z_fpoint_arraylist_removelast(wpoint_arraylist *l);

    wpoints_array *z_fpoint_arraylist_append_new(wpoint_arraylist *l, float maxwidth, float minwidth);

    wpoints_array *z_new_fpoint_array(int initsize, float maxwidth, float minwidth);

    void z_drop_fpoint_arraylist(wpoint_arraylist *l);

    void z_drop_fpoint_array(wpoints_array *a);

    void z_fpoint_arraylist_append(wpoint_arraylist *l, wpoints_array *a);

    wpoints_array *z_keep_fpoint_array(wpoints_array *a);

    void z_fpoint_add_xyw(wpoints_array *a, float x, float y, float w);

    wpoints_array *z_auto_increase_fpoints_array(wpoints_array *a);

    void z_fpoint_add(wpoints_array *a, wfpoint p);

    wpoints_array *z_resize_fpoints_array(wpoints_array *a, int size);

    wpoint_arraylist *z_new_fpoint_arraylist();

    wpoint_arraylist *z_keep_fpoint_arraylist(wpoint_arraylist *l);

    spoint get_last_inserted() {
        spoint p = {0, 0};
        if (m_cur_path) {
            p = m_cur_path->last_point;
        }
        return p;
    }

private:

    wpoint_arraylist *m_arr_list = nullptr;
public:
    wpoint_arraylist *getM_arr_list() const;

private:
    wpoints_array *m_cur_path = nullptr;
    float m_w_max{7}, m_w_min{4};
public:
    float getM_w_max() const;
};

float z_movespeed(tpoint s, tpoint e);

float z_distance(spoint s, spoint e);

float z_linewidth(tpoint b, tpoint e, float w, float step);


typedef struct z_list_node_s z_list_node;
struct z_list_node_s {
    void *data;
    z_list_node *n;
    z_list_node *p;
};

typedef void *(*z_list_node_alloc_fun)();

typedef void(*z_list_node_drop_fun)(void *data);


struct z_list_s {
    z_list_node_alloc_fun alloc;
    z_list_node_drop_fun drop;
    z_list_node *first;
    z_list_node *last;
};
typedef struct z_list_s z_list;

z_list *z_list_new(z_list_node_alloc_fun allocfun, z_list_node_drop_fun dropfun);

void *z_list_append_new(z_list *zlist);

void *z_list_remove_last(z_list *zlist);

void z_list_clear(z_list *zlist);

void z_list_free(z_list *zlist);


#endif

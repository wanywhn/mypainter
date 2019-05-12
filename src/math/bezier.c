#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include "bezier.h"

#define min(X,Y) ((X) < (Y) ? (X) : (Y))

#define z_malloc_struct(t) (t*)calloc(1, sizeof(t))
static void* z_malloc_array(unsigned int count, unsigned int size);
static void* z_resize_array(void *p, size_t count, size_t size);

static void z_fpoint_array_set_last_info(wpoints_array *arr, spoint last_point, float last_width);

static float z_square(float f){ return (float)f*f; };
static float z_cubic_(float f){ return (float)powf(f, 3); };

typedef struct z_bezier_factors_s {
    float bezier_step;      // must be divisible by 1.0f
	float max_width_diff;   // max width diff between two near lines
    float max_move_speed;   // 
    float max_linewith;     
} z_bezier_factors ;

int z_point_equals(spoint *p1, spoint *p2) {
    return (p1->x==p2->x&&p1->y==p2->y) ? 1 : 0;
}

wpoints_array *z_keep_fpoint_array(wpoints_array *a) {
    if(a) a->ref++;
    return a;
}

void z_drop_fpoint_array(wpoints_array *a) {
    if(!a) return;

    if( !(--(a->ref)) ) {
        free(a);
    }
}

wpoint_arraylist *z_keep_fpoint_arraylist(wpoint_arraylist *l) {
    if(!l) return NULL;
    l->ref++;
    return l;
}

void z_drop_fpoint_arraylist(wpoint_arraylist *l) {
    if(!l) return;

    if( !(--(l->ref)) ) {
        wpoint_arraylist_node *c = l->first;
        wpoint_arraylist_node *n;
        while(c) {
            z_drop_fpoint_array(c->a);
            n = c->n;
            free(c);
            c = n;
        }
    } 
}

static const float defualt_max_width = 100.0f;
static const float default_min_width = 5.0f;

wpoints_array *z_new_fpoint_array(int initsize, float maxwidth, float minwidth) {
    if(initsize<=0) return NULL;
    wpoints_array *a = malloc(sizeof(wpoints_array));
    a->point = z_malloc_array(initsize, sizeof(wfpoint));
    a->ref = 1;
    a->len = 0;

    if(maxwidth<0 || minwidth<0 || maxwidth<minwidth ){
        maxwidth = defualt_max_width;
        minwidth = default_min_width;
    }

    a->maxwidth = maxwidth;
    a->minwidth = minwidth;

    a->cap = initsize;
    return a;
}

wpoints_array *z_resize_fpoints_array(wpoints_array* a, int count){
    if(!a || count<=0) return NULL;

    a->point = (wfpoint*)z_resize_array(a->point, count, sizeof(wfpoint));
    a->cap = count;
    a->len = min(a->cap, a->len);
    return a;
}

wpoint_arraylist *z_new_fpoint_arraylist() {
    wpoint_arraylist *l = z_malloc_struct(wpoint_arraylist);
    l->ref = 1;
    l->first = l->end = NULL;
    return l;
}

void z_fpoint_arraylist_append(wpoint_arraylist *l, wpoints_array *a) {
    wpoint_arraylist_node *node = z_malloc_struct(wpoint_arraylist_node);
    node->a = z_keep_fpoint_array(a);
    node->n = NULL;

    if(!l->first) {
        l->first = node;
    }
    else {
        l->end->n = node;
    }

    l->end = node;
}

wpoints_array *z_fpoint_arraylist_append_new(wpoint_arraylist *l, float max, float min) {
    wpoints_array *a = z_new_fpoint_array(24, max, min);
    z_fpoint_arraylist_append(l, a);
//	printf("append new points array\n");
    return a; 
}

void z_fpoint_arraylist_removelast(wpoint_arraylist *l) {
    
    wpoint_arraylist_node *c = l->first;

    z_drop_fpoint_array(l->end->a);
    free(l->end);

    while(c->n != l->end) { c = c->n; }

    c->n = NULL;
    l->end = c; 
}

wpoints_array *z_auto_increase_fpoints_array(wpoints_array *a) {
    int cap = a->cap + (a->cap+3)/4;
    return z_resize_fpoints_array(a, cap);
}

float z_movespeed(tpoint s, tpoint e) {
	float d = z_distance(s.p, e.p);
    return (0==d) ? 0 : d/(e.t-s.t);
}

float z_distance(spoint b, spoint e){
    return (float)sqrtf( z_square(e.x-b.x) + z_square(e.y-b.y) );
}

void  z_fpoint_add_xyw(wpoints_array *a, float x, float y, float w)  {
	if( !a || (a->point[a->len-1].p.x==x && a->point[a->len-1].p.y==y) ) return;
	
    if(a->len==a->cap) 
        z_auto_increase_fpoints_array(a);

	wfpoint *p = a->point + (a->len++);
	p->p.x = x; p->p.y = y; p->w = w;
}

void  z_fpoint_add(wpoints_array *a, wfpoint p) {
    z_fpoint_add_xyw(a, p.p.x, p.p.y, p.w);
}

void  z_fpoint_differential_add(wpoints_array *a, wfpoint p) {
    if(!a) return; 

    if( a->len==0 ) {
        z_fpoint_add(a, p);
        return;
    }

// #define bad_show
#ifdef bad_show
    z_fpoint_add(a, p);
	return;
#endif
	float max_diff = 0.1f;
    wfpoint *last = a->point + (a->len-1);
    spoint sp = last->p;
	float sw = last->w;

	int n = (int)((fabsf(p.w - last->w) / max_diff) + 1);
    float x_step = (p.p.x - sp.x) / n;
    float y_step = (p.p.y - sp.y) / n;
    float w_step = (p.w - sw)	  / n;

    int i;
    for(i=0; i<(n-1); i++ ){
		sp.x += x_step;
		sp.y += y_step;
		sw += w_step;
        z_fpoint_add_xyw(a, sp.x, sp.y, sw);
    }
	z_fpoint_add(a, p);
}

void  z_square_bezier(wpoints_array *a, wfpoint b, spoint c, wfpoint e){
	if(!a) return;
	const float f = 0.1f;
	for(float t=f; t<=1.0; t+=f ) {
		float x1 = z_square(1-t)*b.p.x + 2*t*(1-t)*c.x + z_square(t)*e.p.x;
		float y1 = z_square(1-t)*b.p.y + 2*t*(1-t)*c.y + z_square(t)*e.p.y;
		float w = b.w + (t* (e.w-b.w));
		wfpoint pw = { {x1, y1}, w};
		z_fpoint_differential_add(a, pw);
	}
}

float z_linewidth(tpoint b, tpoint e, float bwidth, float step) {
	const float max_speed = 5.0f;
	float d = z_distance(b.p, e.p);
	//TODO 获取dpi
	float s = d / (e.t - b.t); s = s > max_speed ? max_speed : s;
	float w = (max_speed-s) / max_speed;
	float max_dif = d * step;
	if( w<0.1f ) w = 0.1f;
	if( fabs( w-bwidth ) > max_dif ) {
		if( w > bwidth )
			w = bwidth + max_dif;
		else
			w = bwidth - max_dif;
	}
	 printf("d:%.4f, time_diff:%f, speed:%.4f, width:%.4f\n", d, e.t-b.t, s, w);
	return w;
}


float z_insert_point(wpoints_array *arr, spoint new_point) {

	if(!arr) return 0;
    int len = arr->len;

//	spoint zp = {new_point.x, new_point.y};
	if( 0==len ){
		wfpoint p = {new_point, 1};
        z_fpoint_add(arr, p); 
		z_fpoint_array_set_last_info(arr, new_point, p.w);
		return p.w;
	}

	float cur_ms = clock()*(1000.0f/CLOCKS_PER_SEC);
	float last_width = arr->last_width;
	float last_ms = arr->last_ms;
	spoint last_point = arr->last_point;

	printf("cur_ms - last_ms = %f\n", cur_ms - last_ms);
	//TODO 两次采样时间小于25毫秒, 或者距离小于2个像素, 不采样计算!!!
	//TODO 起始伐值，可以控制第一笔的大小
	float distance = z_distance(new_point, last_point);
	if( (cur_ms-last_ms) < 0.3|| distance < 2) {
		return 0;
	}

	float step = arr->len > 4 ? 0.05f : 0.2f;
	tpoint bt = { {last_point.x,last_point.y}, last_ms};
	tpoint et = { new_point, cur_ms};
	float w = (z_linewidth(bt, et, last_width, step) + last_width) / 2;
	wpoints_array *points = z_new_fpoint_array(51, arr->maxwidth, arr->minwidth);
    wfpoint generated_lastpoint = arr->point[len-1];
	z_fpoint_add(points, generated_lastpoint);

	if( 1==len ) {
		wfpoint p = { {(bt.p.x + et.p.x + 1) / 2, (bt.p.y + et.p.y +1) / 2}, w};
        z_fpoint_differential_add(points, p);
		w = p.w;
	}
	else {
		wfpoint bw = generated_lastpoint;
		spoint c =  {last_point.x,last_point.y};
		//TODO k=0.5
		wfpoint ew = {{(last_point.x + new_point.x)/2, (last_point.y + new_point.y)/2}, w};
		z_square_bezier(points, bw, c, ew);
	}
	
	// escape the first spoint
    int i;
	for(i=1; i<points->len; i++) {
        z_fpoint_add(arr, points->point[i]);
	}

	z_drop_fpoint_array(points); 
	z_fpoint_array_set_last_info(arr, new_point, w);

	return w;
}

void z_insert_last_point(wpoints_array *arr, spoint e) {
	if(!arr) return;
	long len= arr->len;
	if(len==0 ) return;
	wpoints_array *points = z_new_fpoint_array(51, arr->maxwidth, arr->minwidth);
	wfpoint zb = arr->point[len-1];
	z_fpoint_add(points, zb);
	
	wfpoint ze = { {e.x, e.y}, 0.1f};
    z_fpoint_differential_add(points, ze);
    int i;
	for(i=1; i<points->len; i++) {
        z_fpoint_add(arr, points->point[i]);
	}
	z_drop_fpoint_array(points);
}

z_list *z_list_new(z_list_node_alloc_fun allocfun, z_list_node_drop_fun dropfun)
{
    z_list *l = NULL;
	l = z_malloc_struct(z_list);
	l->alloc = allocfun;
	l->drop = dropfun;
	l->first = l->last = NULL;
    return l;
}

void *z_list_append_new(z_list *zlist) 
{
    z_list_node *node = NULL;
    void *data = NULL;

    if(!zlist->alloc || !zlist->drop) 
        return NULL;

	node = z_malloc_struct(z_list_node);
	node->data = zlist->alloc();
	node->n = NULL;
	node->p = NULL;

    if(node) {
        if(!zlist->first) {
            zlist->first = zlist->last = node;
        }
        else {
            node->n = NULL;
            node->p = zlist->last;
            zlist->last->n = node; 
            zlist->last = node;
        } 
        data = node->data;
    }

    return data;
}
void *z_list_remove_last(z_list *zlist) 
{
    void *data = NULL;
    z_list_node *tmp = zlist->last;
    if(zlist->last) {
        tmp = zlist->last;
        if(zlist->last==zlist->first){
            zlist->last = zlist->first = NULL;
        }
        else {
            zlist->last = tmp->p;
            zlist->last->n = NULL;
        }
    }

    if(tmp) {
        data = tmp->data; 
        free(tmp);
    }

    return data; 
}

void z_list_clear(z_list *zlist) 
{
	while (zlist->first)
		zlist->drop(z_list_remove_last(zlist));
}

void z_list_free(z_list *zlist) 
{
	z_list_clear(zlist);
	free(zlist);
}


void* z_malloc_array(unsigned int count, unsigned int size) { 
	unsigned int totalsize = count * size;
	if (totalsize <= 0) return 0;

	void *buffer = malloc(count * size);
	if(buffer) memset(buffer, 0, count * size);
	return buffer;
}

void* z_resize_array(void *p, size_t count, size_t size) {
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
	arr->last_ms = clock()*(1000.0f/CLOCKS_PER_SEC);
	arr->last_width = last_width; 
//	printf("reset last ms to 0x%li\n", arr->last_ms);
}


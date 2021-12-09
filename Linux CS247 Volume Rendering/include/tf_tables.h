#ifndef __TF_TABLES_h__
#define __TF_TABLES_h__


// transfer function tables

float tf0[ 512 ];

static float tf1[ 8 ]= {
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

static float tf2[ 16 ]= {
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

static float tf3[ 48 ]= {
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.15f, 0.66f, 1.0f, 0.0f,
	0.2f, 0.66f, 1.0f, 0.35f,
	0.25f, 0.66f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

static float tf4[ 48 ]= {
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.66f, 0.66f, 0.0f,
	1.0f, 0.66f, 0.66f, 0.35f,
	1.0f, 0.66f, 0.5f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

static float tf5[ 48 ]= {
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.5f, 0.0f,
	0.0f, 0.5f, 0.5f, 0.01f,
	0.0f, 0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f, 0.2f,
	0.5f, 0.5f, 0.0f, 0.5f,
	0.5f, 0.5f, 0.0f, 0.2f,
	0.5f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.8f
};


#endif // __TF_TABLES_h__
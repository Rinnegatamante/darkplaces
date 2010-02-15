
// This code written in 2010 by Forest Hale (lordhavoc ghdigital com), and placed into public domain.

// Based on information in http://zach.in.tu-clausthal.de/papers/vrst02.html (in particular vrst02_boxtree.pdf)

#ifndef BIH_H
#define BIH_H

typedef enum biherror_e
{
	BIHERROR_OK, // no error, be happy
	BIHERROR_OUT_OF_NODES, // could not produce complete hierarchy, maxnodes too low (should be roughly half of numleafs)
}
biherror_t;

typedef enum bih_nodetype_e
{
	BIH_SPLITX = 0,
	BIH_SPLITY = 1,
	BIH_SPLITZ = 2,
	BIH_LEAF = 3,
}
bih_nodetype_t;

typedef struct bih_node_s
{
	bih_nodetype_t type; // = BIH_SPLITX and similar values
	// TODO: store just one float for distance, and have BIH_SPLITMINX and BIH_SPLITMAXX distinctions, to reduce memory footprint and traversal time, as described in the paper (vrst02_boxtree.pdf)
	// TODO: move bounds data to parent node and remove it from leafs?
	float mins[3];
	float maxs[3];
	// < 0 is a leaf index (-1-leafindex), >= 0 is another node index (always >= this node's index)
	int children[2];
}
bih_node_t;

typedef struct bih_leaf_s
{
	bih_nodetype_t type; // = BIH_LEAF
	float mins[3];
	float maxs[3];
	// data past this point is generic and entirely up to the caller...
	int textureindex;
	int itemindex; // triangle or brush index
}
bih_leaf_t;

typedef struct bih_s
{
	// permanent fields
	// leafs are constructed by caller before calling BIH_Build
	int numleafs;
	bih_leaf_t *leafs;
	// nodes are constructed by BIH_Build
	int numnodes;
	bih_node_t *nodes;

	// fields used only during BIH_Build:
	int maxnodes;
	int error; // set to a value if an error occurs in building (such as numnodes == maxnodes)
	int *leafsort;
	int *leafsortscratch;
}
bih_t;

int BIH_Build(bih_t *bih, int numleafs, bih_leaf_t *leafs, int maxnodes, bih_node_t *nodes, int *temp_leafsort, int *temp_leafsortscratch);

#endif
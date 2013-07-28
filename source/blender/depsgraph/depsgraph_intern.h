/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2013 Blender Foundation.
 * All rights reserved.
 *
 * Original Author: Joshua Leung
 * Contributor(s): None Yet
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 * API's for internal use in the Depsgraph
 *
 * XXX: is this file really needed? Or does its purpose overlap with others?
 *      For now, let's keep this on the assumption that these API's aren't
 *      good for any of the other headers.
 */ 
 
#ifndef __DEPSGRAPH_INTERN_H__
#define __DEPSGRAPH_INTERN_H__

/* includes for safety! */
#include <stdarg.h>

/* Graph Building/Low-Level Querying =============================== */

/* Node Querying --------------------------------------------------- */

/* Find node which matches the specified description
 *
 * < graph: dependency graph that node will be part of
 * < id: ID block that is associated with this
 * < type: type of node we're dealing with
 * < name: custom identifier assigned to node 
 *
 * > returns: A node matching the required characteristics if it exists
 *            OR NULL if no such node exists in the graph
 */
DepsNode *DEG_find_node(Depsgraph *graph, ID *id, eDepsNode_Type type, const char name[]);


/* Get the node with data matching the requested characteristics
 * ! New nodes are created if no matching nodes exist...
 * ! Arguments are as for DEG_find_node()
 *
 * > returns: A node matching the required characteristics that exists in the graph
 */
DepsNode *DEG_get_node(Depsgraph *graph, ID *id, eDepsNode_Type type, const char name[]);

/* Get the node referred to by data path
 * ! This is just a convenience wrapper for DEG_get_node() 
 *   when all we have is a ID + RNA Path
 *
 * < graph: Depsgraph to find node from
 * < id: ID-Block that path is rooted on
 * < path: RNA-Path to resolve
 * > returns: (IDDepsNode | DataDepsNode) as appropriate
 */
// XXX: needs type arg for filtering what we return?
DepsNode *DEG_get_node_from_rna_path(Depsgraph *graph, const ID *id, const char path[]);

/* Node Management ---------------------------------------------------- */

/* Create a new node, but don't do anything else with it yet... 
 * ! Ensuring that the node is properly initialised is the responsibility
 *   of whoever is calling this...
 *
 * > returns: The new node created (of the specified type), but which hasn't been added to
 *            the graph yet (callers need to do this manually, as well as other initialisations)
 */
DepsNode *DEG_create_node(eDepsNode_Type type);

/* Add given node to graph 
 * < (id): ID-Block that node is associated with (if applicable)
 */
void DEG_add_node(Depsgraph *graph, DepsNode *node, ID *id);

/* Create a new (outer) node and add to graph
 * ! Arguments are as for DEG_find_node()
 *
 * > returns: The new node created (of the specified type) which now exists in the graph already
 *            (i.e. even if an ID node was created first, the inner node would get created first)
 */
DepsNode *DEG_add_new_node(Depsgraph *graph, ID *id, eDepsNode_Type type, const char name[]);

/* Make a (deep) copy of provided node and it's little subgraph
 * ! Newly created node is not added to the existing graph
 */
DepsNode *DEG_copy_node(const DepsNode *node);

/* Remove node from graph, but don't free any of its data */
void DEG_remove_node(Depsgraph *graph, DepsNode *node);

/* Free node data but not node itself 
 * ! Node data must be separately freed by caller
 * ! DEG_remove_node() should be called before calling this...
 */
void DEG_free_node(DepsNode *node)

/* Groups ------------------------------------------------------------- */

/* Make a group from the two given outer nodes 
 * < node1: (DatablockDepsNode | GroupDepsNode)
 * < node2: (DatablockDepsNode)
 * > return: (GroupDepsNode) either a new group node, or node1 if that was a group already
 */
DepsNode *DEG_group_cyclic_node_pair(Depsgraph *graph, DepsNode *node1, DepsNode *node2);

/* Relationships Handling ============================================== */

/* Convenience Macros -------------------------------------------------- */

/* Helper macros for interating over set of relationship
 * links incident on each node.
 *
 * NOTE: Since each relationship is shared between the two nodes
 *       involved, each node must use "LinkData" to reference
 *       the nodes nearby...
 *
 * NOTE: it is safe to perform removal operations here...
 *
 * < first_link: (LinkData *) first LinkData in list of relationships (in/out links)
 * > rel:  (DepsRelation *) identifier where DepsRelation that we're currently accessing comes up
 */
#define DEPSNODE_RELATIONS_ITER_BEGIN(first_link, relation)                    \
	{                                                                                \
		LinkData *__rel_iter, *__rel_next;                                           \
		for (__rel_iter = first_link; __rel_iter; __rel_iter = __rel_next) {         \
			DepsRelation *relation = (DepsRelation *)__rel_iter->data;               \
			__rel_next = __rel_iter->next;

			/* ... code for iterator body can be written here ... */

#define DEPSNODE_RELATIONS_ITER_END                                                  \
		}                                                                            \
	}(void)

/* API Methods --------------------------------------------------------- */

/* Create new relationship object, but don't add it to graph yet */
DepsRelation *DEG_create_new_relation(DepsNode *from, DepsNode *to, 
                                      eDepsRelation_Type type, 
                                      const char *description);

/* Add given relationship to the graph */
void DEG_add_relation(Depsgraph *graph, DepsRelation *rel);


/* Add new relationship between two nodes */
DepsRelation *DEG_add_new_relation(Depsgraph *graph, DepsNode *from, DepsNode *to,
                                   eDepsRelation_Type type, const char *description);


/* Make a copy of given relationship */
DepsRelation DEG_copy_relation(const DepsRelation *src);


/* Remove relationship from graph, but don't free it yet */
void DEG_remove_relation(Depsgraph *graph, DepsRelation *rel);

/* Free relationship's data 
 * ! Assumes that it isn't part of graph anymore (DEG_remove_relation() called)
 * ! Relationship itself *is* freed...
 */
void DEG_free_relation(DepsRelation *rel);

/* Node Types Handling ================================================= */

/* "Typeinfo" for Node Types ------------------------------------------- */

/* Typeinfo Struct (nti) */
typedef struct DepsNodeTypeInfo {
	/* Identification ................................. */
	eDepsNode_Type type;           /* DEPSNODE_TYPE_### */
	size_t size;                   /* size in bytes of the struct */
	char name[MAX_NAME];           /* name of node type */
	
	/* Data Management ................................ */
	/* Initialise node-specific data - the node already exists */
	void (*init_data)(DepsNode *node, ID *id, StructRNA *srna, void *data);
	
	/* Free node-specific data, but not node itself */
	// XXX: note - this should not try to call remove_from_graph()...
	void (*free_data)(DepsNode *node);
	
	/* Make a copy of "src" node's data over to "dst" node */
	void (*copy_data)(DepsNode *dst, const DepsNode *src);
	
	/* Add node to graph */
	void (*add_to_graph)(Depsgraph *graph, DepsNode *node, ID *id);
	
	/* Remove node from graph - Only use when node is to be replaced... */
	void (*remove_from_graph)(Depsgraph *graph, DepsNode *node);
	
	/* Querying ...................................... */
	
	/* Does node match the (outer-node) data-type requirements? */
	bool (*match_outer)(DepsNode *node, ID *id, StructRNA *srna, void *data);
	
	// ...
	
	/* Graph Building (Outer nodes only) ............. */
	/* Generate atomic operation nodes (inner nodes subgraph) */
	void (*build_subgraph)(DepsNode *node);
	
	// TODO: perform special pruning operations to cull branches which don't do anything?
} DepsNodeTypeInfo;

/* Typeinfo Management -------------------------------------------------- */

/* Register node type */
void DEG_register_node_typeinfo(DepsNodeTypeInfo *typeinfo);

/* Get typeinfo for specified type */
DepsNodeTypeInfo *DEG_get_node_typeinfo(eDepsNode_Type type);

/* Get typeinfo for provided node */
DepsNodeTypeInfo *DEG_node_get_typeinfo(DepsNode *node);


#endif // __DEPSGRAPH_INTERN_H__

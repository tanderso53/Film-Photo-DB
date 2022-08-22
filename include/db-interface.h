/**
 * @file db-interface.h
 * @author Tyler J. Anderson
 * @copyright (2022) Tyler J. Anderson use under BSD 3-clause license
 * @brief C API for accessing data from the Film Photo DB
 */

#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#ifndef DM_PROPERTY_LIMIT
#define DM_PROPERTY_LIMIT 64
#endif

#ifndef DM_FIELDSIZE
#define DM_FIELDSIZE 512
#endif

typedef enum fpdbi_object_type {
	FPDBI_ROLL,
	FPDBI_FRAME,
	FPDBI_ENTITY
} fpdbi_object_type_t;

typedef enum fpdbi_property_type {
	FPDBI_UNSIGNED_INT,
	FPDBI_INT,
	FPDBI_TEXT,
	FPDBI_BOOL,
	FPDBI_UNSIGNED_LONG,
	FPDBI_LONG,
	FPDBI_UNSIGNED_SHORT,
	FPDBI_SHORT,
	FPDBI_DATETIME
} fpdbi_property_type_t;

typedef enum fpdbi_metadata_type {
	FPDBI_META_ENTITY_TYPE,
	FPDBI_META_LAB,
	FPDBI_META_PROCESS,
	FPDBI_META_ASSIGNMENT,
	FPDBI_META_CAMERA,
	FPDBI_META_LENS,
	FPDBI_META_FILM,
	FPDBI_META_DATA_TYPE
} fpdbi_metadata_type_t;

typedef struct fpdbi_server_node {
	char constring[128];
} fpdbi_server_t;

typedef struct fpdbi_property_tuple_node {
	char property[DM_FIELDSIZE];
	char value[DM_FIELDSIZE];
	fpdbi_property_type_t type;
	struct fpdbi_property_tuple_node *prev;
	struct fpdbi_property_tuple_node *next;
} fpdbi_property_tuple_t;

typedef struct fpdbi_metadata_node {
	char name[DM_FIELDSIZE];
	char description[DM_FIELDSIZE];
	fpdbi_metadata_type_t type;
	fpdbi_property_tuple_t *properties;
	struct fpdbi_metadata_node *prev;
	struct fpdbi_metadata_node *next;
	fpdbi_property_tuple_t _property_storage[DM_PROPERTY_LIMIT];
} fpdbi_metadata_t;

typedef struct fpdbi_object_node {
	char name[DM_FIELDSIZE];
	char description[DM_FIELDSIZE];
	fpdbi_object_type_t type;
	fpdbi_metadata_t *metadata_list;
	fpdbi_property_tuple_t *properties;
	struct fpdbi_object_node *pref;
	struct fpdbi_object_node *next;
	struct fpdbi_object_node *parent;
	fpdbi_metadata_t _metadata_storage[16];
	fpdbi_property_tuple_t _property_storage[DM_PROPERTY_LIMIT];
} fpdbi_object_t;

/** @brief Create an object of a given type
 *
 * Creates a template object of the given type initialized with
 * templates for the parent and metadata objects apropriate for the
 * given object type.
 *
 * @param obj Pointer to the object to initialize
 * @param type The type of object to create
 * @param parent The parent object to attach the new object to. If
 * NULL, the new object will not be attached to a parent
 *
 * @return Pointer to the new object on success, or NULL on failure
 */
fpdbi_object_t *fpdbi_create_object(fpdbi_object_t *obj,
				    fpdbi_object_type_t type,
				    fpdbi_object_t *parent);

/** @brief Get list of children of the provided parent object
 *
 * @param parent Pointer to parent object
 * @param buf Pointer to first element of array to copy list data to
 * @param maxlen Maximum number of elements that can be written
 *
 * @return Number of elements written or -1 * error code if there was
 * an error
 */
long fpdbi_find_children(fpdbi_object_t *parent, fpdbi_object_t *buf,
			 unsigned int maxlen);

/** @brief Find a list of objects of the type matching the optional filter
 *
 * @param objtype type of the object to find
 * @param buf The pointer to the first element of the array to write
 * the object list to
 * @param maxlen The maximum length of data to store
 * @param filter SQL filter expressions, ignore if NULL
 *
 * @return Number of results on success, -1 * error code on failure
 */
long fpdbi_find_objects_by_type(fpdbi_object_type_t objtype,
				fpdbi_object_t *buf,
				unsigned int maxlen, const char *filter);

/** @brief Find a list of metadata of the type matching the optional filter
 *
 * @param objtype type of the metadata to find
 * @param buf The pointer to the first element of the array to write
 * the metadata list to
 * @param maxlen The maximum length of data to store
 * @param filter SQL filter expressions, ignore if NULL
 *
 * @return Number of results on success, -1 * error code on failure
 */
long fpdbi_find_metadata_by_type(fpdbi_metadata_type_t mdtype,
				 fpdbi_metadata_t *buf, unsigned int maxlen,
				 const char *filter);

/** @brief Overwrites the destination object with the source object
 *
 * @return A ptr to the destination object if successful, NULL on
 * failure
 */
fpdbi_object_t *fpdbi_replace_object_template(fpdbi_object_t *objdest,
					      const fpdbi_object_t *objsrc);

/** @brief Overwrites the destination metadata with the source metadata
 *
 * @return A ptr to the destination metadata if successful,
 * NULL on failure
 */
fpdbi_metadata_t fpdbi_replace_metadata_template(fpdbi_metadata_t *metadest,
						 const fpdbi_metadata_t *metasrc);

/** @brief Appends the given as a new object in the database
 *
 * @return The row id of the added object or -1 * error code on failure
 */
long fpdbi_commit_object_append(const fpdbi_object_t *obj);

/** @brief Replaces an object in the database with supplied object
 *
 * @return The row id of the replaced object or -1 * error code on failure
 */
long fpdbi_commit_object_replace(const fpdbi_object_t *newobj,
				 const fpdbi_object_t *oldobj);

/** @brief Removes an object in the database matching the id of the
 * supplied object
 * @return The row id of the removed object or -1 * error code on failure
 */
long fpdbi_commit_object_remove(const fpdbi_object_t *obj);

/** @brief Appends the given as a new metadata in the database
 *
 * @return The row id of the added metadata or -1 * error code on failure
 */
long fpdbi_commit_metadata_append(const fpdbi_metadata_t *obj);

/** @brief Replaces an metadata in the database with supplied metadata
 *
 * @return The row id of the replaced metadata or -1 * error code on failure
 */
long fpdbi_commit_metadata_replace(const fpdbi_metadata_t *newobj,
				 const fpdbi_metadata_t *oldobj);

/** @brief Removes an metadata in the database matching the id of the
 * supplied metadata
 * @return The row id of the removed metadata or -1 * error code on failure
 */
long fpdbi_commit_metadata_remove(const fpdbi_metadata_t *obj);

/** @brief Initialize database server
 *
 * @return 0 on success, -1 * error code on failure
 */
int fpdbi_server_init(fpdbi_server_t *server, const char *constring);

#endif /* #ifndef DB_INTERFACE_H */

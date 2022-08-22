/**
 * @file db-model.hxx
 * @author Tyler J. Anderson
 * @copyright (2022) Tyler J. Anderson BSD 3-clause License
 * @brief Film Photo DB object model as C++ objects
 */

#ifndef DB_MODEL_HXX
#define DB_MODEL_HXX

#include <ctime>

#ifndef DM_FIELDSIZE
#define DM_FIELDSIZE 512
#endif

#ifndef DM_PROPERTYLIMIT
#define DM_PROPERTYLIMIT 64
#endif

namespace data_model {

  struct dm_table {
    unsigned long id;
    char name[DM_FIELDSIZE];
    char description[DM_FIELDSIZE];
    virtual ~dm_table() = 0;
  };

  namespace metadata_schema {

    struct entity_type : public dm_table {
      bool digital;
      unsigned int max_instances;
      char extended_props[DM_FIELDSIZE][DM_PROPERTYLIMIT];
      char inherited_props[DM_FIELDSIZE];
    };

    struct lab : public dm_table {
      char name[DM_FIELDSIZE];
      char description[DM_FIELDSIZE];
      char street_address[DM_FIELDSIZE];
      char web_address[DM_FIELDSIZE];
      char email_address[DM_FIELDSIZE];
      char phone_customer_service[DM_FIELDSIZE];
    };

    struct process : public dm_table {
    };

    struct assignment : public dm_table {
      char location[DM_FIELDSIZE];
      struct tm start_date;
      struct tm end_date;
    };

    struct camera : public dm_table {
      char manufacturer[DM_FIELDSIZE];
      char model[DM_FIELDSIZE];
      bool lens_removeable;
    };

    struct lens : public dm_table {
      char manufacturer[DM_FIELDSIZE];
      char model[DM_FIELDSIZE];
      bool built_in;
      char f_number[DM_FIELDSIZE];
      char focal_length[DM_FIELDSIZE];
    };

    struct film : public dm_table {
      char manufacturer[DM_FIELDSIZE];
      bool color;
      bool negative;
      char listed_process[DM_FIELDSIZE];
      unsigned int iso;
      unsigned int listed_ei[10];
    };

    struct data_type : public dm_table {
    };
  }

  namespace object_schema {

    struct roll : public dm_table {
      metadata_schema::lab *lab = NULL;
      metadata_schema::process *process = NULL;
      struct tm developed_date;
      char old_roll_ident[DM_FIELDSIZE];
      char lab_roll_ident[DM_FIELDSIZE];
    };

    struct frame : public dm_table {
      object_schema::roll *roll = NULL;
      int frame_num;
      struct tm date_time_stamp;
      metadata_schema::assignment *assignment = NULL;
      metadata_schema::camera *camera = NULL;
      metadata_schema::lens *lens = NULL;
      metadata_schema::film *film = NULL;
      unsigned int ei;
    };

    struct entity : public dm_table {
      object_schema::frame *frame = NULL;
      metadata_schema::entity_type *entity_type = NULL;
      struct tm date_time_stamp;
      char series[DM_FIELDSIZE];
      char extended_props[DM_FIELDSIZE][DM_PROPERTYLIMIT];

      struct entity_type_properties : public dm_table {
	metadata_schema::entity_type *entity_type = NULL;
	metadata_schema::data_type data_type;
      } properties[DM_PROPERTYLIMIT];

      struct entity_property_values : public dm_table {
	entity_type_properties *entity_type_properties = NULL;
	object_schema::entity *entity = NULL;
      } values[DM_PROPERTYLIMIT];
    };
  }
}

#endif /* #ifndef DB_MODEL_HXX */

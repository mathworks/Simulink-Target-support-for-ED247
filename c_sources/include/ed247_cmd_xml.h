/*
 * Copyright 2020 The MathWorks, Inc.
 */
 
 #ifndef __ED247_CMD_XML__
 #define __ED247_CMD_XML__
 
 #include <string.h>
 #include <stdio.h>
 #include <libxml/parser.h>
 #include <libxml/xmlreader.h>

/************
 * Includes *
 ************/ 
 #include "ed247.h"
 #include "tools.h"

/***********
 * Defines *
 ***********/
 #ifdef  LIBED247_EXPORTS
 #define LIBED247_EXPORT 	__declspec(dllexport)
 #else
 #undef  LIBED247_EXPORT
 #define LIBED247_EXPORT
 #endif
 
 #define MAX_A429_BUS			512
 #define MAX_A429_MSG_PER_BUS	512
 #define MAX_A664_MESSAGES		512
 #define MAX_A825_MESSAGES		512
 #define MAX_NAD_MESSAGES		512

 #define A429_DATA_TYPE			SS_UINT8 // = 2
 #define A429_DATA_SIZE 		4
 #define A429_DATA_SAMPLE_SIZE	(4*sizeof(char))
 #define A664_DATA_TYPE			SS_UINT8
 #define A664_DATA_SIZE			4
 #define A825_DATA_TYPE			SS_UINT8
 #define A825_DATA_SIZE			4

/*********
 * Types *
 *********/ 
 typedef enum {
	XML_NODE_NONE = 0,
	XML_NODE_ELEMENT,
	XML_NODE_ATTRIBUTE,
	XML_NODE_TEXT,
	XML_NODE_CDATA,
	XML_NODE_ENTITY_REFERENCE,
	XML_NODE_ENTITY,
	XML_NODE_PROCESSING_INSTRUCTION,
	XML_NODE_COMMENT,
	XML_NODE_DOCUMENT,
	XML_NODE_DOCUMENT_TYPE,
	XML_NODE_DOCUMENT_FRAGMENT,
	XML_NODE_NOTATION,
	XML_NODE_WHITESPACE,
	XML_NODE_SIGNIFICANT_WHITESPACE,
	XML_NODE_END_ELEMENT,
	XML_NODE_END_ENTITY,
	XML_NODE_DECLARATION,
 } xml_node_type_t;
 
 typedef enum {
	 A429_MESSAGE_SAMPLING = 0,
	 A429_MESSAGE_QUEUING,
	 A429_MESSAGE_UNKNOWN
 } a429_message_type_t;
 
 typedef enum {
	 CMD_COUNT_READ_OK = 0,
	 CMD_COUNT_CANNOT_OPEN_FILE,
 } cmd_count_status_t;

 typedef enum {
	 CMD_READ_OK = 0,
	 CMD_READ_INVALID_FILE,
	 CMD_READ_CANNOT_CREATE_CONTEXT,
	 CMD_READ_CANNOT_OPEN_FILE,
 } cmd_read_status_t;

 typedef enum {
	 CMD_ALLOCATION_OK = 0,
	 CMD_MALLOC_FAILURE,
	 CMD_A429_MALLOC_FAILURE,
	 CMD_A429_MESSAGE_MALLOC_FAILURE,
	 CMD_A664_MALLOC_FAILURE,
	 CMD_A825_MALLOC_FAILURE,
	 CMD_NAD_MALLOC_FAILURE,
 } cmd_allocate_memory_status_t;
 
 typedef enum {
	 CMD_FREE_OK = 0,
 } cmd_free_memory_status_t;
 
 typedef struct {
	 ed247_direction_t 			direction;
	 a429_message_type_t		type;
	 char 						name[STRING_MAX_LENGTH];
	 char 						label[STRING_MAX_LENGTH];
	 char 						sdi[STRING_MAX_LENGTH];
	 uint32_t 					period_us;
	 uint32_t 					validity_duration_us;
	 char 						comment[STRING_MAX_LENGTH];
 } cmd_data_a429_message_t;

 typedef struct {
	 char 						name[STRING_MAX_LENGTH];
	 int 						n_messages;
	 cmd_data_a429_message_t	messages[MAX_A429_MSG_PER_BUS];
 } cmd_data_a429_t;

 typedef struct {
	 ed247_direction_t 			direction;
	 char 						name[STRING_MAX_LENGTH];
	 size_t 					sample_max_size_bytes;
	 uint32_t 					period_us;
	 uint32_t 					validity_duration_us;
	 char 						comment[STRING_MAX_LENGTH];
 } cmd_data_a664_t;

 typedef struct {
	 char 						name[STRING_MAX_LENGTH];
	 size_t 					sample_max_number;
	 uint32_t 					period_us;
	 uint32_t 					validity_duration_us;
	 char 						comment[STRING_MAX_LENGTH];
 } cmd_data_a825_t;

 typedef struct {
	 ed247_direction_t 			direction;
	 char 						name[STRING_MAX_LENGTH];
	 ed247_nad_type_t 			type;
	 char 						comment[STRING_MAX_LENGTH];
 } cmd_data_nad_t;
 
 typedef struct {
	 int 	a429;
	 int 	a664;
	 int 	a825;
	 int 	nad;
 } cmd_counter_t;
 
 typedef struct {
	 cmd_counter_t 				counter;
	 cmd_data_a429_t 			a429[MAX_A429_BUS];
	 cmd_data_a664_t 			a664[MAX_A664_MESSAGES];
	 cmd_data_a825_t 			a825[MAX_A825_MESSAGES];
	 cmd_data_nad_t  			nad[MAX_NAD_MESSAGES];
 } cmd_data_t;
 
 
/**********
 * Common *
 **********/ 
namespace ed247simulink {

	class Cmd {

		private:
			Tools* _tools;

		public:
			Cmd();
			Cmd(Tools* tools);

		public:
			cmd_read_status_t readData(const char *filename, cmd_data_t *data);

		protected:
			void fillA429(xmlNodePtr node, cmd_data_a429_t *data); 
			void fillA429Message(xmlNodePtr node, cmd_data_a429_message_t *data);
			void fillNAD(xmlNodePtr node, cmd_data_nad_t *data);
			void fillA664(xmlNodePtr node, cmd_data_a664_t *data);
			void fillA825(xmlNodePtr node, cmd_data_a825_t *data);

		private:
			// TODO : Replace by ed247_* functions
			ed247_direction_t local_direction_from_string(const char *direction);
			ed247_nad_type_t local_nad_type_from_string(const char *type);
			// END TODO
			a429_message_type_t local_a429_message_type_from_string(const char *type);

	};

}

 #endif
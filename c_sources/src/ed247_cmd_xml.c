/*
 * Copyright 2020 The MathWorks, Inc.
 */
 
 #include "ed247_cmd_xml.h"
 
/**
 * cmd_read_data:
 * @filename: the file name to parse
 *
 * Parse information about an XML file.
 */
 LIBED247_EXPORT cmd_read_status_t cmd_read_data(const char *filename, cmd_data_t *data){

	xmlParserCtxtPtr	_p_xml_context;
	xmlDocPtr 			_p_xml_doc;
	xmlNodePtr 			xmlRootNode;
	xmlNodePtr 			xml_node_iter;

	// Check that file exist
	if (fileexists(filename) != 0){
		return CMD_READ_INVALID_FILE;
	}

	// Create context
	if((_p_xml_context = xmlNewParserCtxt()) == NULL){/* polyspace RTE:UNR [Justified:Low] Robustness */
		myprintf("Failed to create context\n");
		return CMD_READ_CANNOT_CREATE_CONTEXT;
	}

	// Create read file
	if((_p_xml_doc = xmlCtxtReadFile(_p_xml_context,filename,NULL,0)) == NULL){/* polyspace RTE:UNR [Justified:Low] Robustness */
		myprintf("Failed to read %s\n",filename);
		if(_p_xml_context){
			xmlFreeParserCtxt(_p_xml_context);
		}
		return CMD_READ_CANNOT_OPEN_FILE;
	}

	// Get root node
	xmlRootNode = xmlDocGetRootElement(_p_xml_doc);

	memset(&(data->counter),0,sizeof(data->counter));
	for (xml_node_iter = xmlRootNode->children; xml_node_iter != NULL; xml_node_iter = xml_node_iter->next){

		if (strcmp(xml_node_iter->name, "A429_Bus") == 0){
			fillA429(xml_node_iter, (data->a429 + data->counter.a429));
			data->counter.a429++;
		} else if (strcmp(xml_node_iter->name, "A664_SamplingMessage") == 0){
			fillA664(xml_node_iter, (data->a664 + data->counter.a664));
			data->counter.a664++;
		} else if (strcmp(xml_node_iter->name, "A825_SamplingMessage") == 0){
			fillA825(xml_node_iter, (data->a825 + data->counter.a825));
			data->counter.a825++;
		} else if (strcmp(xml_node_iter->name, "NAD_Variable") == 0){
			fillNAD(xml_node_iter, (data->nad + data->counter.nad));
			data->counter.nad++;
		}

	}

	// Free memory
	if(_p_xml_doc){/* polyspace RTE:UNR [Justified:Low] Robustness */
		xmlFreeDoc(_p_xml_doc);
	}
	if(_p_xml_context){/* polyspace RTE:UNR [Justified:Low] Robustness */
		xmlFreeParserCtxt(_p_xml_context);
	}

	return CMD_READ_OK;

 }

/*
 * MEMORY ALLOCATION
 */
 LIBED247_EXPORT cmd_allocate_memory_status_t cmd_allocate_memory(cmd_data_t **data){

	int i;

	if (((*data)=(cmd_data_t*)malloc(sizeof(cmd_data_t))) == NULL) {
		return CMD_MALLOC_FAILURE;
	}

	if (((*data)->a429=(cmd_data_a429_t*)malloc(sizeof(cmd_data_a429_t) * MAX_A429_BUS)) == NULL) {
		return CMD_A429_MALLOC_FAILURE;
	}
	for (i = 0; i<MAX_A429_BUS; i++){
		if (((*data)->a429[i].messages=(cmd_data_a429_message_t*)malloc(sizeof(cmd_data_a429_message_t) * MAX_A429_MSG_PER_BUS)) == NULL) {
			return CMD_A429_MESSAGE_MALLOC_FAILURE;
		}
	}

	if (((*data)->a664=(cmd_data_a664_t*)malloc(sizeof(cmd_data_a664_t) * MAX_A664_MESSAGES)) == NULL) {
		return CMD_A664_MALLOC_FAILURE;
	}

	if (((*data)->a825=(cmd_data_a825_t*)malloc(sizeof(cmd_data_a825_t) * MAX_A825_MESSAGES)) == NULL) {
		return CMD_A825_MALLOC_FAILURE;
	}

	if (((*data)->nad=(cmd_data_nad_t*)malloc(sizeof(cmd_data_nad_t) * MAX_NAD_MESSAGES)) == NULL) {
		return CMD_NAD_MALLOC_FAILURE;
	}

	return CMD_ALLOCATION_OK;

 }
 
 LIBED247_EXPORT cmd_free_memory_status_t cmd_free_memory(cmd_data_t *data){

	int i;

	if (data->a664 != NULL) {
		free(data->a664);
	}
/*
	for (i = 0; i<data->counter.a429 && i<MAX_A429_BUS; i++){
		if (data->a429[i].messages != NULL) {
			free(data->a429[i].messages);
		}
	}
*/
	if (data->a429 != NULL) {
		free(data->a429);
	}

	if (data->nad != NULL) {
		free(data->nad);
	}

	if (data != NULL) {/* polyspace RTE:UNR [Justified:Low] Robustness */
		free(data);
	}

	return CMD_FREE_OK;

 }
 
/* 
 * STATIC FUNCTIONS
 */
 void fillA429(xmlNodePtr node, cmd_data_a429_t *data){

	const char 		*attr_name;
	char 			*attr_value;
	struct _xmlAttr *xml_attr;
	xmlNodePtr		xml_node_iter;

	// Node attributes
	for(xml_attr = node->properties ; xml_attr != NULL ; xml_attr = xml_attr->next){ 
		attr_name = xml_attr->name;
		attr_value = xmlGetProp(xml_attr->parent,xml_attr->name);
		if (strcmp(attr_name, "Name") == 0){
			strcpy(data->name,attr_value);
		}
	}

	// Node children (messages)
	data->n_messages = 0;
	for (xml_node_iter = node->children; xml_node_iter != NULL; xml_node_iter = xml_node_iter->next){

		if (xml_node_iter->type == XML_NODE_ELEMENT){
			fillA429Message(xml_node_iter, (data->messages + data->n_messages));
			(data->n_messages)++;
		}

	}

 }
 
 void fillA429Message(xmlNodePtr node, cmd_data_a429_message_t *data){

	const char *attr_name;
	char *attr_value;
	struct _xmlAttr *xml_attr;

	data->type = local_a429_message_type_from_string(node->name);

	for(xml_attr = node->properties ; xml_attr != NULL ; xml_attr = xml_attr->next){ 

		attr_name = xml_attr->name;
		attr_value = xmlGetProp(xml_attr->parent,xml_attr->name);
		if (strcmp(attr_name, "Direction") == 0){
			data->direction = local_direction_from_string(attr_value);
		} else if (strcmp(attr_name, "Name") == 0){
			strcpy(data->name,attr_value);
		} else if (strcmp(attr_name, "Label") == 0){
			strcpy(data->label,attr_value);
		} else if (strcmp(attr_name, "SDI") == 0){
			strcpy(data->sdi,attr_value);
		} else if (strcmp(attr_name, "PeriodUs") == 0){	
			data->period_us = atoi(attr_value);
		} else if (strcmp(attr_name, "ValidityDurationUs") == 0){
			data->validity_duration_us = atoi(attr_value);
		} else if (strcmp(attr_name, "Comment") == 0){
			strcpy(data->comment,attr_value);
		}

	}

 }
 
 void fillA664(xmlNodePtr node, cmd_data_a664_t *data){

	const char *attr_name;
	char *attr_value;
	struct _xmlAttr *xml_attr;

	for(xml_attr = node->properties ; xml_attr != NULL ; xml_attr = xml_attr->next){ 
		attr_name = xml_attr->name;
		attr_value = xmlGetProp(xml_attr->parent,xml_attr->name);
		if (strcmp(attr_name, "Direction") == 0){
			data->direction = local_direction_from_string(attr_value);
		} else if (strcmp(attr_name, "Name") == 0){
			strcpy(data->name,attr_value);
		} else if (strcmp(attr_name, "SampleMaxSizeBytes") == 0){
			data->sample_max_size_bytes = atoi(attr_value);
		} else if (strcmp(attr_name, "PeriodUs") == 0){
			data->period_us = atoi(attr_value);
		} else if (strcmp(attr_name, "ValidityDurationUs") == 0){
			data->validity_duration_us = atoi(attr_value);
		} else if (strcmp(attr_name, "Comment") == 0){
			strcpy(data->comment,attr_value);
		}

	}

 }

 void fillA825(xmlNodePtr node, cmd_data_a825_t *data){

	const char *attr_name;
	char *attr_value;
	struct _xmlAttr *xml_attr;

	for(xml_attr = node->properties ; xml_attr != NULL ; xml_attr = xml_attr->next){ 
		attr_name = xml_attr->name;
		attr_value = xmlGetProp(xml_attr->parent,xml_attr->name);
		if (strcmp(attr_name, "Name") == 0){
			strcpy(data->name,attr_value);
		} else if (strcmp(attr_name, "SampleMaxNumber") == 0){
			data->sample_max_number = atoi(attr_value);
		} else if (strcmp(attr_name, "PeriodUs") == 0){	
			data->period_us = atoi(attr_value);
		} else if (strcmp(attr_name, "ValidityDurationUs") == 0){
			data->validity_duration_us = atoi(attr_value);
		} else if (strcmp(attr_name, "Comment") == 0){
			strcpy(data->comment,attr_value);
		}

	}

 }
 
 void fillNAD(xmlNodePtr node, cmd_data_nad_t *data){

	const char *attr_name;
	char *attr_value;
	struct _xmlAttr *xml_attr;

	for(xml_attr = node->properties ; xml_attr != NULL ; xml_attr = xml_attr->next){ 
		attr_name = xml_attr->name;
		attr_value = xmlGetProp(xml_attr->parent,xml_attr->name);
		if (strcmp(attr_name, "Direction") == 0){
			data->direction = local_direction_from_string(attr_value);
		} else if (strcmp(attr_name, "Name") == 0){
			strcpy(data->name,attr_value);
		} else if (strcmp(attr_name, "Type") == 0){
			data->type = local_nad_type_from_string(attr_value);
		} else if (strcmp(attr_name, "Comment") == 0){
			strcpy(data->comment,attr_value);
		}

	}

 }
 
/*
 * HELPERS
 */ 

 ed247_direction_t local_direction_from_string(const char *direction){
	if(strcmp(direction,"In") == 0){
		return ED247_DIRECTION_IN;
	}else if(strcmp(direction,"Out") == 0){
		return ED247_DIRECTION_OUT;
	}else if(strcmp(direction,"InOut") == 0){
		return ED247_DIRECTION_INOUT;
	}else{
		return ED247_DIRECTION__INVALID;
	}
}

ed247_nad_type_t local_nad_type_from_string(const char *type)
{
	if(strcmp(type,"int8") == 0){
		return ED247_NAD_TYPE_INT8;
	}else if(strcmp(type,"int16") == 0){
		return ED247_NAD_TYPE_INT16;
	}else if(strcmp(type,"int32") == 0){
		return ED247_NAD_TYPE_INT32;
	}else if(strcmp(type,"int64") == 0){
		return ED247_NAD_TYPE_INT64;
	}else if(strcmp(type,"uint8") == 0){
		return ED247_NAD_TYPE_UINT8;
	}else if(strcmp(type,"uint16") == 0){
		return ED247_NAD_TYPE_UINT16;
	}else if(strcmp(type,"uint32") == 0){
		return ED247_NAD_TYPE_UINT32;
	}else if(strcmp(type,"uint64") == 0){
		return ED247_NAD_TYPE_UINT64;
	}else if(strcmp(type,"float32") == 0){
		return ED247_NAD_TYPE_FLOAT32;
	}else if(strcmp(type,"float64") == 0){
		return ED247_NAD_TYPE_FLOAT64;
	}else{
		return ED247_NAD_TYPE__INVALID;
	}
}

a429_message_type_t local_a429_message_type_from_string(const char *type){
	if(strcmp(type,"SamplingMessage") == 0){
		return A429_MESSAGE_SAMPLING;
	}else if(strcmp(type,"QueuingMessage") == 0){
		return A429_MESSAGE_QUEUING;
	}else{
		return A429_MESSAGE_UNKNOWN;
	}
}


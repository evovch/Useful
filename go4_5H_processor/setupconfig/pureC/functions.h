#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h" // for stc_mapping, stc_setup_config

// ================================================================================================
// stc_mapping ====================================================================================
// ================================================================================================

void InitStcMapping(stc_mapping* ptr);

void DumpStcMapping(const stc_mapping* ptr);

// ================================================================================================
// stc_setup_config ===============================================================================
// ================================================================================================

void InitStcSetupConfig(stc_setup_config* ptr);

void DestructStcSetupConfig(stc_setup_config* ptr);

void DumpStcSetupConfig(const stc_setup_config* ptr);

void ExtendMappingsListStcSetupConfig(stc_setup_config* ptr, const stc_mapping* ptrMapping);

void ImportXML(stc_setup_config* ptr, const char* filename);

#endif // FUNCTIONS_H

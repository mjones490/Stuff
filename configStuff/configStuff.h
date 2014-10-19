#ifndef CONFIG_STUFF_H
#define CONFIG_STUFF_H

struct config_struct;

struct config_struct *config_open(char *filename);
void config_close(struct config_struct *config);

const char *config_value(struct config_struct *config);
const char *config_key(struct config_struct *config);
const char *config_section(struct config_struct *config);

void config_next_section(struct config_struct *config);
void config_next_key(struct config_struct *config);
#endif

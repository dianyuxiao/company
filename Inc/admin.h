#ifndef __ADMIN_H__
#define __ADMIN_H__

typedef enum {
	PwdIsSelect,
	CardIsSelect
}Select_Area;

void EnterAdminSet(void);

void AddPassword(void);

void DelPassword(void);

void AddCardNumber(void);

void DelCardNumber(void);
#endif




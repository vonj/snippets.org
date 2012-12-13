#ifndef CCARD_H
#define CCARD_H

enum ccard_type_ty
{
      ccard_type_unknown,
      ccard_type_mastercard,
      ccard_type_visa,
      ccard_type_amex,
      ccard_type_diners,
      ccard_type_discover,
      ccard_type_enroute,
      ccard_type_jcb,
      ccard_type_bankcard
};
typedef enum ccard_type_ty ccard_type_ty;

enum ccard_error_ty
{
      ccard_error_none = 0,
      ccard_error_type_unknown,
      ccard_error_non_numeric,
      ccard_error_too_long,
      ccard_error_length_incorrect,
      ccard_error_checksum
};
typedef enum ccard_error_ty ccard_error_ty;


char *ccard_type_name (ccard_type_ty);
char *ccard_error_name (ccard_error_ty);
ccard_error_ty ccard_valid (char *, ccard_type_ty *);

#endif /* CCARD_H */

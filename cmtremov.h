/*
 * CMTREMOV.H
 * Comment remover class.
 *
 * ver 1.0, 30 Jun 1996
 *
 * Public domain by:
 *   Jari Laaksonen
 *   Arkkitehdinkatu 30 A 2
 *   FIN-33720 Tampere
 *   FINLAND
 *
 *   Fidonet : 2:221/360.20
 *   Internet: jla@to.icl.fi
 */

#ifndef _CMTREMOV_H_
#define _CMTREMOV_H_

#include "cmtparsr.h"

class CommentRemover : public CommentParser
{
public:

protected:
    virtual void ProcessActions (Event theEvent);

};

#endif // _CMTREMOV_H_


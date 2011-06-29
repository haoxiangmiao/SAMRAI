/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   An abstract base class for a DatabaseFactory 
 *
 ************************************************************************/

#ifndef included_tbox_DatabaseFactory
#define included_tbox_DatabaseFactory

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/Database.h"

namespace SAMRAI {
namespace tbox {

/**
 * @brief Abstract base class factory used to build Database objects.
 *
 * Used to build database objects.  For example, RestartManager
 * may use a DatabaseFactory to build databases when creating
 * a restart file.
 */
class DatabaseFactory:public DescribedClass
{
public:
   /*
    * Build a new Database instance.
    */
   virtual Pointer<Database>
   allocate(
      const std::string& name) = 0;
};

}
}

#endif
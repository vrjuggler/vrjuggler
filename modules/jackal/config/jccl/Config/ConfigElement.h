/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_CONFIG_ELEMENT_H_
#define _JCCL_CONFIG_ELEMENT_H_

#include <jccl/jcclConfig.h>
#include <vector>

#include <sstream>
#include <cppdom/cppdom.h>
#include <vpr/Util/Assert.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/Config/ConfigDefinitionPtr.h>

namespace jccl
{

/** \class ConfigElement ConfigElement.h jccl/Config/ConfigElement.h
 *
 * A unit of configuration information.
 * References a config element DOM node and a definition for the given
 * configuration element type.
 */
class JCCL_CLASS_API ConfigElement
{
public:
   /** Constructor. */
   ConfigElement();

   /** Constructs a ConfigElement matching the given definition.
    *  @param def Points to a valid ConfigDefinition.
    */
   ConfigElement(ConfigDefinitionPtr def);

   /** Copy constructor. */
   ConfigElement(const ConfigElement& c);

   /** Destroys a ConfigElement and frees associated memory.
    *  @post Self has been destroyed, along with all properties
    *        and values associated with it (but _not_ the memory
    *        associated with its ConfigDefinition).
    */
   ~ConfigElement();

   bool isValid() const;

   /**
    * Initializes this element from cppdom::Node.
    * Constructs the element to reference the given element node.
    * @post This element is set up and initialized.
    * @returns false if failed to initialize.
    */
   bool initFromNode(cppdom::NodePtr elementNode);

   /** Assignment operator. */
   ConfigElement& operator=(const ConfigElement& c);

   /** Tests for value equality of two ConfigElements.
    *  @return True iff self and c have the same values for all properties.
    */
   bool operator==(const ConfigElement& c) const;

   /** Inequality. */
   bool operator!=(const ConfigElement& c) const;

   /** Lexical comparison based on element names (alphabetically). */
   bool operator<(const ConfigElement& c) const;

   /**
    * Gets a child element from a property of self.
    * @param path The complete name of a child element in self.
    *             The form is, for example, "property_token/instance_name".
    *
    * @note This uses the name attribute of the elements.
    *
    * @return The ConfigElementPtr that is the child element.  This may be
    *         a NULL ConfigElementPtr if the child element name was
    *         not found.  Check before use.
    */
   ConfigElementPtr getChildElement(const std::string &path);

   /**
    * Writes a representation of self to out.
    * @param out  An ostream.
    * @param self A ConfigElement.
    */
   friend JCCL_API(std::ostream&) operator<<(std::ostream& out,
                                             const ConfigElement& self);

   /**
    * Returns the number of values for the specified property.
    *
    * @pre The named property exists in the definition for this config element.
    *
    * @param property The token of a property.  This comes from the
    *                 definition for this config element.
    *
    * @return The number of values that exist for the given property which
    *         may be 0.
    */
   unsigned int getNum(const std::string& property) const;

   /**
    * Returns the instance name of this ConfigElement without any hierarchy
    * information.
    *
    * @see getFullName
    */
   std::string getName() const;

   /**
    * Returns the fully qualified, unique name of this element.  This will be
    * different from getName() when this element is a child of another element.
    * In that case, the name will be based on the element hierarchy and the
    * property token.  The format in that case will be
    * "element name 0/property_token_0/element name 1/property_token_1/..."
    */
   std::string getFullName() const;

   /**
    * Returns the version number of the definition used by this ConfigElement.
    */
   unsigned int getVersion() const;

   /** Returns the string that identifies self's ConfigDefinition. */
   std::string getID() const;

   /**
    * Returns a value from one of self's properties.
    *
    * @param prop The token string for a property.
    * @param ind  The index to the property's list of values.  Use
    *             getNum() to determine the number of values for a
    *             given property.  The valid range is 0 to getNum()-1.
    *
    * @note Call with a template parameter equal to the type to return
    *        ie. getProperty<int>("int_property", 3);
    *
    * @todo This function could throw I/O exceptions.  Maybe catch those and
    *       turn them into something JCCL'ish.
    */
   template<class T>
   T getProperty(const std::string& prop, int ind) const
   {
      T ret_val = T();
      std::istringstream iss(getPropertyString(prop, ind));
      iss >> ret_val;
      return ret_val;
   }

   /**
    * Gets value that defaults to property 0.
    *
    * @note Can't use default param because GCC doesn't
    *       allow specialization in a declaration.
    *       In other words we couldn't use a default value for the property
    *       index within the std::string and ConfigElementPtr specializations.
    */
   template<class T>
   T getProperty(const std::string& prop) const;

   /**
    * Sets a value for the given property.
    *
    * @param prop The token string for a property.
    * @param ind  The index to the property's list of values.
    * @param val  The value to set.
    *
    * @note If property does not exist yet, then we create it in the element.
    */
   template<class T>
   bool setProperty(const std::string& prop, const int ind, T val)
   {
      cppdom::NodePtr cdata_node = getPropertyCdataNode(prop, ind, true);
      vprASSERT(cdata_node.get() != NULL && "Autogrow failed");

      std::ostringstream oss;
      oss << val;
      cdata_node->setCdata(oss.str());
      return true;
   }

   /**
    * Specialization for boolean values.
    */
   bool setProperty(const std::string& prop, const int ind, bool val);

   /** Specialization for ConfigElementPtrs */
   bool setProperty(const std::string& prop, const int ind,
                    ConfigElementPtr val);

   /**
    * Returns a list of self's depenencies.
    * Dependencies are any config elements named by an "Element Pointer"
    * property of self (or any element embedded in self).
    *
    * @return A vector of the names of all config elements referenced by self,
    *         which can be used for dependency checking.
    */
   std::vector<std::string> getElementPtrDependencies() const;

   /**
    * Return a list of self's child (embedded) elements.
    *
    * @return A vector of ConfigElementPtr objects that are the config
    *         elements embedded in self.
    */
   std::vector<jccl::ConfigElementPtr> getChildElements() const;

   /** Associates the definintion d with this element.
    *  @post Set the definintion for this element.
    */
   void setDefinition(ConfigDefinitionPtr d);
   
   /** Returns the Config Definition for this ConfigElement. */
   ConfigDefinitionPtr getConfigDefinition();

   cppdom::NodePtr getNode() const;

protected:
   /**
    * Returns the string value of the given property.
    *
    * @post  If property is found, then return the contents.  Else return "".
    *
    * @param prop The token string for the property.
    * @param ind  The index of the property.
    */
   std::string getPropertyString(const std::string& prop, int ind) const;

   /**
    * Get the property's cdata node.
    *
    * @param prop  The property token.
    * @param ind   The index inside the property.
    * @param autoCreate If true, then autocreate the property node to use.
    *
    * @note We always autocreate the cdata node if need be.
    */
   cppdom::NodePtr getPropertyCdataNode(const std::string& prop, int ind,
                                        bool autoCreate) const;

   /**
    * Gets a boolean value from the given property.
    * Have to call this way because std::stringstring does not convert "true"
    * and "false" into corresponding bool values.
    */
   bool getProperty_bool(const std::string& prop, int ind) const;

   /**
    * Gets an element ptr from the given property.
    * Have to call this way because specialization would use symbols that
    * aren't available.
    */
   ConfigElementPtr getProperty_ElementPtr(const std::string& prop,
                                           int ind) const;

protected:
   cppdom::NodePtr     mNode;  /**< Node for the config element. */
   ConfigDefinitionPtr mDef;   /**< Definition for this element. */
   bool                mValid; /**< Flag to signal whether element is valid. */
};

template<>
inline std::string ConfigElement::getProperty<std::string>(const std::string& prop, int ind) const
{
   return getPropertyString(prop, ind);
}

/**
 * Specialization for booleans so that we can read "true" and "false" rather
 * than "1" and "0".
 */
template<>
inline bool ConfigElement::getProperty<bool>(const std::string& prop, int ind) const
{
   return getProperty_bool(prop, ind);
}

/**
 * Specialization for ConfigElementPtr's.
 * @note Needed for child elements.
 */
template<>
inline ConfigElementPtr ConfigElement::getProperty<ConfigElementPtr>(const std::string& prop, int ind) const
{
   return getProperty_ElementPtr(prop,ind);
}

template<class T>
inline T ConfigElement::getProperty(const std::string& prop) const
{
   return getProperty<T>(prop, 0);
}

} // namespace jccl

#endif

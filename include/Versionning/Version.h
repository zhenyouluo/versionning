
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
  * @file
  *
  * @mainpage
  * @author Jeremy Coulon
  * @date 2012
  *
  * @section Introduction
  *
  * This C++ library contains a simple portable data structure to store versionning information of a project.\n
  * It allows you to store information about your library being in version vX.Y.Z.T.\n
  * It is particularly useful for comparing version number.
  * For example to know if version vX.Y.Z.T is greater than v3.12.0.0.\n
  *
  * A version number consists of 4 unsigned numbers :
  * @li The first number is called 'Major' number.
  * @li The second number is called 'Minor' number.
  * @li The third number is called 'Patch' number.
  * @li The fourth (last) number is called 'Tweak' number.
  *
  * @section Installation
  *
  * This library is a header-only library. That means you don't need to compile anything to use it in your own project. You can just write :
  * @code
  * #include <Versionning/Version.h>
  * @endcode
  *
  * @section License
  *
  *          Copyright Jeremy Coulon 2012.\n
  * Distributed under the Boost Software License, Version 1.0.\n
  *   (See accompanying file <a href="../LICENSE_1_0.txt">LICENSE_1_0.txt</a> or copy at\n
  *         http://www.boost.org/LICENSE_1_0.txt)
  *
  * @section Examples
  *
  */

#pragma once

//===================
//==  Versionning  ==
//===================
#include "Versionning/VersionException.h"

//=============
//==  Boost  ==
//=============
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

//===========
//==  STD  ==
//===========
#include <string>
#include <vector>

/// Namespace of the Versionning library
namespace Vers
{
    /// Data structure to store a version number
    class Version
    {
    public:
        /// Default constructor
        /**
          * Version is initialized to v0.0.0.0
          */
        Version()
            : major_(0), minor_(0), patch_(0), tweak_(0)
        {

        }

        /// Constructor taking 4 numbers as parameters
        /**
          * @param major    1st version number
          * @param minor    2nd version number
          * @param patch    3rd version number
          * @param tweak    4th version number
          */
        Version(unsigned short major, unsigned short minor, unsigned short patch, unsigned short tweak)
            : major_(major), minor_(minor), patch_(patch), tweak_(tweak)
        {

        }

        /// Constructor from a string
        /**
          * @param version a string representing a version number. This string must be well formed.
          * @see setVersion()
          */
        Version(const std::string& version)
        {
            setVersion(version);
        }

        /// Get 1st version number
        unsigned short getMajor() const
        {
            return major_;
        }

        /// Get 2nd version number
        unsigned short getMinor() const
        {
            return minor_;
        }

        /// Get 3rd version number
        unsigned short getPatch() const
        {
            return patch_;
        }

        /// Get 4th version number
        unsigned short getTweak() const
        {
            return tweak_;
        }

        /// Get complete version number as a string
        std::string getVersion() const
        {
            static const std::string separator(".");
            std::string result;
            result += boost::lexical_cast<std::string>(major_);
            result += separator;
            result += boost::lexical_cast<std::string>(minor_);
            result += separator;
            result += boost::lexical_cast<std::string>(patch_);
            result += separator;
            result += boost::lexical_cast<std::string>(tweak_);
            return result;
        }

        /// Set 1st version number
        void setMajor(unsigned short major)
        {
            major_ = major;
        }

        /// Set 2nd version number
        void setMinor(unsigned short minor)
        {
            minor_ = minor;
        }

        /// Set 3rd version number
        void setPatch(unsigned short patch)
        {
            patch_ = patch;
        }

        /// Set 4th version number
        void setTweak(unsigned short tweak)
        {
            tweak_ = tweak;
        }

        /// Set complete version number from a string
        /**
          * The given string must be well formed.
          * It must be composed of 1 to 4 unsigned numbers separated by a dot '.' or comma ','.
          * If the string is bad formed, an exception is thrown.
          * @param version a string containing the version number
          * @throw VersionException
          */
        void setVersion(const std::string& version)
        {
            std::vector<std::string> splitResult;
            boost::algorithm::split(splitResult, version, boost::algorithm::is_any_of(".,"));
            // Check that we have between 1 and 4 numbers in the version string
            if(splitResult.size() == 0 || splitResult.size() > 4)
                throw VersionException("Incorrect version string: " + version + " . The provided string should contains 1-4 numbers separated by '.' or ','.");

            boost::algorithm::trim(splitResult[0]);
            major_ = boost::lexical_cast<unsigned short>(splitResult[0]);

            if(splitResult.size() >= 2)
            {
                boost::algorithm::trim(splitResult[1]);
                minor_ = boost::lexical_cast<unsigned short>(splitResult[1]);
            }
            else
                minor_ = 0;

            if(splitResult.size() >= 3)
            {
                boost::algorithm::trim(splitResult[2]);
                patch_ = boost::lexical_cast<unsigned short>(splitResult[2]);
            }
            else
                patch_ = 0;

            if(splitResult.size() == 4)
            {
                boost::algorithm::trim(splitResult[3]);
                tweak_ = boost::lexical_cast<unsigned short>(splitResult[3]);
            }
            else
                tweak_ = 0;
        }

    private:
        unsigned short major_;
        unsigned short minor_;
        unsigned short patch_;
        unsigned short tweak_;
    };

    /// Equal operator of 2 version numbers
    /**
      * @param v1   First version number
      * @param v2   Second version number
      * @return True if the 2 versions are equal. False otherwise.
      */
    inline bool operator== (const Version& v1, const Version& v2)
    {
        return v1.getMajor() == v2.getMajor()
                && v1.getMinor() == v2.getMinor()
                && v1.getPatch() == v2.getPatch()
                && v1.getTweak() == v2.getTweak();
    }

    /// Not-Equal operator of 2 version numbers
    /**
      * @param v1   First version number
      * @param v2   Second version number
      * @return True if the 2 versions are different. False otherwise.
      */
    inline bool operator!= (const Version& v1, const Version& v2)
    {
        return !(v1 == v2);
    }

    /// Less-than operator of 2 version numbers
    /**
      * @param v1   First version number
      * @param v2   Second version number
      * @return True if the first version number is less than the second version number. False otherwise.
      */
    inline bool operator< (const Version& v1, const Version& v2)
    {
        if(v1.getMajor() < v2.getMajor())
            return true;
        else if(v1.getMajor() > v2.getMajor())
            return false;
        else
        {
            if(v1.getMinor() < v2.getMinor())
                return true;
            else if(v1.getMinor() > v2.getMinor())
                return false;
            else
            {
                if(v1.getPatch() < v2.getPatch())
                    return true;
                else if(v1.getPatch() > v2.getPatch())
                    return false;
                else
                {
                    if(v1.getTweak() < v2.getTweak())
                        return true;
                    else
                        return false;
                }
            }
        }
    }

    /// Greater-than operator of 2 version numbers
    /**
      * @param v1   First version number
      * @param v2   Second version number
      * @return True if the first version number is greater than the second version number. False otherwise.
      */
    inline bool operator> (const Version& v1, const Version& v2)
    {
        if(v1.getMajor() > v2.getMajor())
            return true;
        else if(v1.getMajor() < v2.getMajor())
            return false;
        else
        {
            if(v1.getMinor() > v2.getMinor())
                return true;
            else if(v1.getMinor() < v2.getMinor())
                return false;
            else
            {
                if(v1.getPatch() > v2.getPatch())
                    return true;
                else if(v1.getPatch() < v2.getPatch())
                    return false;
                else
                {
                    if(v1.getTweak() > v2.getTweak())
                        return true;
                    else
                        return false;
                }
            }
        }
    }

    /// Less-or-equal operator of 2 version numbers
    /**
      * @param v1   First version number
      * @param v2   Second version number
      * @return True if the first version number is less than or equal to the second version number. False otherwise.
      */
    inline bool operator<= (const Version& v1, const Version& v2)
    {
        return !(v1 > v2);
    }

    /// Greater-or-equal operator of 2 version numbers
    /**
      * @param v1   First version number
      * @param v2   Second version number
      * @return True if the first version number is greater than or equal to the second version number. False otherwise.
      */
    inline bool operator>= (const Version& v1, const Version& v2)
    {
        return !(v1 < v2);
    }

}


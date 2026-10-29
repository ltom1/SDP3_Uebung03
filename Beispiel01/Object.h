#ifndef OBJECT_H
#define OBJECT_H

/// @author Thomas Lechner
/// @date 29.10.2026
/// @brief Base class of all base classes.
/// 
/// Is the common root class of all classes. If there is no other base class, 
/// this class is to be selected as the base class.
class Object
{
public:
	/// @brief Virtual default destructor -> once virtual, always virtual.
	/// 
	/// It is not necessary to declare another virtual destructor 
	/// in derived classes.
	virtual ~Object() = default;

protected:
	/// @brief Protected default constructor that makes the class abstract.
	Object() = default;
};

#endif
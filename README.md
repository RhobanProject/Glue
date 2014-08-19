# Glue

## CMake

To use Glue, you should set `GLUE` to the directory of Glue and then include
the `Glue.cmake` file:

```cmake
set(GLUE "/path/to/glue" 
    CACHE STRING "Glue directory")

include("${GLUE}/Glue.cmake")
```

Then, call `glue_parse()` on all headers you want to get annotations:

```cmake
# Adds MyClass to the header parsing process
glue_parse("MyClass.h")
```

You can tell that a type is compatible with another with `glue_is_convertible()`:

```cmake
# Tells that a float can be converted to an int
glue_is_convertible(float int)
```

If you have custom types, add its header using `glue_add_header()`, it will then
be included in the generated files.

## Annotations

### Glue:Block

This annotation tells that a class is a node, parameters are:

* `family`: the family (group) of the node
* `name`: if you want the name to be different of the class name
* `description`: a text explaining what the block does

### Glue:Input, Glue:Output and Glue:Parameter

These annotations tells that the parameter or method is a field. It can
be applied to:

* Attributes of a class
* Methods of the class, can be:
  * Getter with no argument
  * Getter with index argument
  * Setter with just a value as argument
  * Setter as an intex and a value as argument

For instance:

```c++
    /***
     * Glue:Input()
     */
     int value;
```

Will result in a block with just an input "value", when:

```c++
    /***
     * Glue:Input()
     */
     int value(int index);
```

Will result in a block with type int[] (multiple connectors)

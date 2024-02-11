# A Solution for 'Attribute Parser' Problem in HackerRank

This is a solution to the [Attribute Parser](https://www.hackerrank.com/challenges/attribute-parser) problem in HackerRank. 

I know there could be many different solutions(maybe even better), but I chose to do it this way and share it with you 😊

## Implementation Details

- The HRML parser constructs a hierarchical representation of the HRML document using a map of tags and their attributes.
- Tags are represented as structs containing pointers to parent tags, maps of children tags, and maps of attributes.
- Regular expressions are used to extract tag names, attributes, and values from each line of HRML input.
- Queries are processed by traversing the tag hierarchy and searching for the specified attribute.



## How to build
```bash
mkdir build
cd build
cmake ..
make
```
## How to run

```bash
./attribute_parser
``` 

## How to test
```bash
./run_tests.sh
```

> PS: Tests could be written (of course) different ways but in this case this way is enough.

## Question
This challenge works with a custom-designed markup language HRML. In HRML, each element consists of a starting and ending tag, and there are attributes associated with each tag. Only starting tags can have attributes. We can call an attribute by referencing the tag, followed by a tilde, '~' and the name of the attribute. The tags may also be nested.

The opening tags follow the format:

`<tag-name attribute1-name = "value1" attribute2-name = "value2" ...>`

The closing tags follow the format:

`</tag-name>`

The attributes are referenced as:

```
tag1~value  
tag1.tag2~name
```

Given the source code in HRML format consisting of **N** lines, answer **Q** queries. For each query, print the value of the attribute specified. Print "Not Found!" if the attribute does not exist.

### Example

HRML listing:
```
<tag1 value = "value">
<tag2 name = "name">
<tag3 another="another" final="final">
</tag3>
</tag2>
</tag1>
```
Queries:
```
tag1~value
tag1.tag2.tag3~name
tag1.tag2~value
```

> Here, tag2 is nested within tag1, so attributes of tag2 are accessed as `tag1.tag2~<attribute>`. 

Results of the queries are:

```
Query                 Value
tag1~value            "value"
tag1.tag2.tag3~name   "Not Found!"
tag1.tag2.tag3~final  "final"
```
### Input Format

The first line consists of two space separated integers, **N** and **Q**. **N** specifies the number of lines in the HRML source program.**Q** specifies the number of queries.

The following **N** lines consist of either an opening tag with zero or more attributes or a closing tag. There is a space after the tag-name, attribute-name, '=' and value.There is no space after the last value. If there are no attributes there is no space after tag name.

**Q** queries follow. Each query consists of string that references an attribute in the source program.More formally, each query is of the form 

$tag_{i1}$.$tag_{i2}$.$tag_{i3}$....$tag_{im}$~attr-name where m >= 1 and $tag_{i1}$, $tag_{i2}$...$tag_{im}$ are valid tags in the input.

### Constraints

> 1 <= N <= 20

> 1 <= Q <= 20


Each line in the source program contains, at most, 200 characters.
Every reference to the attributes in the **Q** queries contains at most 200 characters.
All tag names are unique and the HRML source program is logically correct, i.e. valid nesting.
A tag can may have no attributes.
Output Format

Print the value of the attribute for each query. Print "Not Found!" without quotes if the attribute does not exist.

### Sample Input

```
4 3
<tag1 value = "HelloWorld">
<tag2 name = "Name1">
</tag2>
</tag1>
tag1.tag2~name
tag1~name
tag1~value
Sample Output
```
```
Name1
Not Found!
HelloWorld
```
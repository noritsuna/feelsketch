# General
## endian
**big endian**


## extension
**fs**


# Headers
## version
**fixed length:1byte(8bits)** version number.


## image size
**fixed length:2byte** Size of the picture used as a base.



## PM-Code Size
**fixed length:1byte** The size of PM-Code.
 It is set to 1/n to the short neighborhood of paper size.



## PM-Code position
**fixed length:1byte** The print position of PM-Code.
|Upper left|1|
|:---------|:|
|The center of the left|2|
|Lower left|3|
|Upper right|4|
|The center of the right|5|
|Lower right|6|
|On a center|7|
|Under a center|8|
|The center of a center|9|



## paper type
**fixed length:1byte** The kind of paper to print.
|size L|1|
|:-----|:|
|Card  |2|
|Post Card|3|
|A3    |4|
|A4    |5|
|B4    |6|
|B5    |7|



## backbround code
**fixed length:1bytes** The code of a background.
 A "color" and a "background image" can be chosen as a background.
 0 has no background.
**A head bit is a flag of printing propriety.**


### color
|transparent|0|none|
|:----------|:|:---|
|brown      |1|#7C6035|
|red        |2|#CC0000|
|orange     |3|#FFB74C|
|yellow     |4|#FFFF00|
|green      |5|#00FF41|
|blue       |6|#0C00CC|
|purple     |7|#C400CC|
|gray       |8|#8C8C8C|
|white      |9|#FAFAFA|
|black      |10|#262626|
|beige      |11|#EAD2AD|
|pink       |12|#F8ABA6|



## reserved
**fixed length:1byte**



## UUID
**fixed length:16bytes(128bits)** ID for identifying the made picture.



## media data
**flexible length:terminal code: The end is  "\r\n(CR LF)"** examples: mail:xxx@pmcode.jp or http://www.pmcode.jp/ssss/



# Commands
## command types
**fixed length:1bytes** The code showing commands (a line, an icon, etc.).
|layer|0|
|:----|:|
|icon |1|
|line |2|
|Bezier curve|3|
|spline curve|4|
|letter|5|
|shape|6|
|terminal|255|



## effect
**fixed length:2bytes** The code showing effects.
 This is expressed in a bit.
|blink|0b0000000000000001|
|:----|:-----------------|
|vibration|0b0000000000000010|
|move |0b0000000000000100|
|fadein|0b0000000000001000|
|fadeout|0b0000000000010000|



## color code
**fixed length:1byte** The code showing colors.
 0 has no background.


### color
|transparent|0|none|
|:----------|:|:---|
|brown      |1|#7C6035|
|red        |2|#CC0000|
|orange     |3|#FFB74C|
|yellow     |4|#FFFF00|
|green      |5|#00FF41|
|blue       |6|#0C00CC|
|purple     |7|#C400CC|
|gray       |8|#8C8C8C|
|white      |9|#FAFAFA|
|black      |10|#262626|
|beige      |11|#EAD2AD|
|pink       |12|#F8ABA6|



## point
**fixed length:2bytes** Coordinates of XY expressed by 1-254.
 It is expressed in order of (x, y).



## icon
**fixed length:2bytes** The code showing icon IDs.



## layer position
**fixed length:1bytes** A head bit is a flag of PM-Code.
 Other than this is the order of a layer.



## line type
**fixed length:1bytes** The code showing pen type.
|name|code|pixel|
|:---|:---|:----|
|A thin line|1   |1    |
|A middle line|2   |3    |
|A thick line|3   |6    |
|Thicker line|4   |10   |
|A thin highlighter pen|5   |3    |
|A middle highlighter pen|6   |6    |
|A thick highlighter pen|7   |10   |



## letter type
**fixed length:1bytes** The code showing font type.



## letter size
**fixed length:1bytes** The code showing font size.



# structure
## header
```
|Version|image size|PM-Code size|PM-Code position|paper type|backbround code|UUID|Reserved|
```


## line
```
|command type(line)|line type|color code|point(1-xy)|point(2-xy)|
```


## line next
```
|point(2-xy)|" is used for "|terminal|
```

**Last "| point 2-xy|" is used for "| point 1-xy|."**


## Bezier curve
```
|command type(Bezier curve)|line type|color code|point(1-xy)|point(2-xy)|point(3-xy)|point(4-xy)|
```


## Bezier curve next
```
|point(2-xy)|point(3-xy)|point(4-xy)|......|terminal|
```

**Last "| point 4-xy|" is used for "| point 1-xy|."**


## spline curve
```
|command type(spline curve)|line type|color code|point(1-xy)|point(2-xy)|point(3-xy)|
```


## spline curve next
```
|point(2-xy)|point(3-xy)|......|terminal|
```

**Last "| point 3-xy|" is used for "| point 1-xy|."**


## icon
```
|command type(icon)|point(1-xy)|icon ID|terminal|
```


## letter
```
|command type(letter)|letter type|letter size|color code|point(1-xy)|UTF-8 code|terminal|
```


**"|point(1-xy) |" is a position of the first character.**



## shape
```
|command type(shape)|line type|color code|point(1-xy)|sharpe ID|terminal|
```

**A built-in template shape.** "|point(1-xy) |" is the starting point.



## layer
```
|command type(layer)|layer position|effect|line|line|icon|
```


**All the commands (a line and an icon) are certainly contained as a part of layer.** There is no termination (terminal) in a layer.



## terminal
```
|command type(terminal)|number:255|
```



# header structure
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   version     |          image size           | PM-Code size  |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  PM-Code pos  |  paper size   |background code|   reserved    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   |                              UUID                             |
   |                                                               |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   |                          media data                           |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |        \r\n(CR LF)            |           body                |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                              body                             |
   |                              body                             |
   |                              body                             |
```


# body structure
## line
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 2   |  line type    |  color code   |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               | command : 255 |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     255       |
   +-+-+-+-+-+-+-+-+
```


## When a line continues
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 2   |  line type    |  color code   |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               | command : 255 |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     255       |
   +-+-+-+-+-+-+-+-+
```


## Bezier curve
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 3   |  line type    |  color code   |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |
   +-+-+-+-+-+-+-+-+
```


## When a Bezier curve continues
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 3   |  line type    |  color code   |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      | command : 255 |     255       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```


## spline curve
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 4   |  line type    |  color code   |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      | command : 255 |     255       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```


## When a spline curve continues
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 4   |  line type    |  color code   |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      |           point               |    point       
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        point      | command : 255 |     255       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```


## icon
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 1   |           point              |     icon id     
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       icon id     | command : 255 |      255     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
```


## letter
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 0   |  layer pos    |           effect              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 5   |  letter type  |  letter size  |  color code   |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |            point              | UTF-8 code    |  UTF-8 code   |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 255 |     255       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```


## terminal
```
    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command : 255 |     255       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```
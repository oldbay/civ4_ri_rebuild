
Набор наблюдений по работе кода
===============================

успешное? создание cmake
------------------------

```
./util_cmake_converter -s ../GIT/GameCore/CvGameCoreDLL/CvGameCoreDLL.2012.sln
```

В linux создание успешное но потребовалась корректировка. 
Ещё нужно будет смотреть всё это в windows!


Точка входа
-----------

 * CvGameCoreDLL.h
 * CvGameCoreDLL.cpp


Глобальные настройки?
---------------------

CvGlobals.h - странно себя ведёт, иногда ломает include в других *.h
CvGlobals.cpp


Ошибки headers
--------------
в CvXMLLoadUtility.h ошибка на объявление CvDLLUtilityIFaceBase -?


CvDLLDataStreamIFaceBase.h
--------------------------
Он пустой - зачем он нужен ???


Python API & SDK
----------------

В python вызывается:

```python
from CvPythonExtensions import *
```

Это обращение к ядру - в sdk python api ядра формируется в CyEnumsInterface.cpp ,
не исключено что гдето ещё? 
Вполне возможно что в sdk далеко не всё api ядра -
либо сам сдк должен буть ещё слинкован с недоступной (закрытой) частью кода.


Определёно большая часть объектов с которые предоставляются из пакета CvPythonExtension в python - 
беруться из sCvDLLPython.cpp!
Остаётся выяснить что помимо представленного в sdk должен отдавать в python CvPythonExtension ?!! 



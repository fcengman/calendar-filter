#!/bin/bash


echo -n  "Test1: ./process_cal3 --start=2021/2/14 --end=2021/2/14 --file=one.ics | diff test01.txt -"
if valgrind --leak-check=yes ./process_cal3 --start=2021/2/14 --end=2021/2/14 --file=one.ics | diff test01.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test2: ./process_cal3 --start=2021/4/18 --end=2021/4/21 --file=two.ics | diff test02.txt -"
if ./process_cal3 --start=2021/4/18 --end=2021/4/21 --file=two.ics | diff test02.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test3: ./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=many.ics | diff test03.txt -"
if  ./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=many.ics | diff test03.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test4: ./process_cal3 --start=2021/4/22 --end=2021/4/23 --file=two.ics | diff test04.txt -"
if  ./process_cal3 --start=2021/4/22 --end=2021/4/23 --file=two.ics | diff test04.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test5: ./process_cal3 --start=2021/2/1 --end=2021/2/1 --file=diana-devops.ics | diff test05.txt -"
if  ./process_cal3 --start=2021/2/1 --end=2021/2/1 --file=diana-devops.ics | diff test05.txt -
then
     echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test6: ./process_cal3 --start=2021/2/2 --end=2021/2/2 --file=diana-devops.ics | diff test06.txt -"
if valgrind --leak-check=yes ./process_cal3 --start=2021/2/2 --end=2021/2/2 --file=diana-devops.ics | diff test06.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test7: ./process_cal3 --start=2021/2/1 --end=2021/2/8 --file=diana-devops.ics | diff test07.txt -"
if ./process_cal3 --start=2021/2/1 --end=2021/2/8 --file=diana-devops.ics | diff test07.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test8: ./process_cal3 --start=2021/2/8 --end=2021/2/15 --file=diana-devops.ics | diff test08.txt -"
if ./process_cal3 --start=2021/2/8 --end=2021/2/15 --file=diana-devops.ics | diff test08.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test9: ./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=diana-devops.ics | diff test09.txt -"
if ./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=diana-devops.ics | diff test09.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test10: ./process_cal3 --start=2021/1/1 --end=2021/4/30 --file=diana-devops.ics | diff test10.txt -"
if ./process_cal3 --start=2021/1/1 --end=2021/4/30 --file=diana-devops.ics | diff test10.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test11: ./process_cal3 --start=2021/5/1 --end=2021/6/1 --file=mlb.ics | diff test11.txt -"
if ./process_cal3 --start=2021/5/1 --end=2021/6/1 --file=mlb.ics | diff test11.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test12: ./process_cal3 --start=2021/5/1 --end=2021/8/1 --file=mlb.ics | diff test12.txt -"
if ./process_cal3 --start=2021/5/1 --end=2021/8/1 --file=mlb.ics | diff test12.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test13: ./process_cal3 --start=2020/12/1 --end=2021/4/30 --file=mlb.ics | diff test13.txt -"
if ./process_cal3 --start=2020/12/1 --end=2021/4/30 --file=mlb.ics | diff test13.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test14: ./process_cal3 --start=2021/8/1 --end=2021/10/1 --file=f1.ics | diff test14.txt -"
if ./process_cal3 --start=2021/8/1 --end=2021/10/1 --file=f1.ics | diff test14.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

echo -n "Test15: ./process_cal3 --start=2021/1/1 --end=2021/12/31 --file=f1.ics | diff test15.txt -"
if ./process_cal3 --start=2021/1/1 --end=2021/12/31 --file=f1.ics | diff test15.txt -
then
    echo " $(tput setaf 2)PASS$(tput sgr 0)"
else
    echo " $(tput setaf 1)FAIL$(tput sgr 0)"
fi

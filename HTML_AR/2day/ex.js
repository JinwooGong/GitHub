function ex01(){
    console.log("hello!");
}
function ex02(){
    console.log("----- ex02 ------");
    var a=10;
    var b=20;
    var c = a+b;
    console.log(c);
    console.log(++c);
    console.log(c++);
    console.log(c);
}
function ex03(){
    console.log("----- ex03 -----");
    var a = "안녕";
    var b = "하세요";
    console.log(a+b);
    console.log(a+1);
    var c = "1" + 1;
    console.log(c);
}

function ex04(){
    console.log("----- ex04 -----");
    var x = 0;
    console.log("           x = ",x);
    x += 10;    console.log("x += 10    x = ",x);
    x -= 5;     console.log("x -= 5     x = ",x);
    x *= 10;    console.log("x *= 10    x = ",x);
    x /= 5;     console.log("x /= 5     x = ",x);
    x %= 3;     console.log("x %= 3     x = ",x);
}

function ex05(){
    console.log("----- ex05 -----");
    var expr1 = true;
    var expr2 = false;
    console.log("expr1 && expr2 : ",expr1 && expr2);
    console.log("expr1 && expr1 : ",expr1 && expr1);
    console.log("expr1 || expr2 : ",expr1 || expr2);
    console.log("expr2 || expr2 : ",expr2 || expr2);
    console.log("!expr1 : ",!expr1);
}

function ex06(){
    console.log("----- ex06 -----");

    var a = 1;
    var b = 3;
    console.log("a : ",a);
    console.log("b : ",b);
    console.log("a == b     : ",a == b);
    console.log("a == '1'   : ",a=='1');
    console.log("a === '1'  : ",a==='1');
    console.log("a != b     : ",a != b);
    console.log("a !== '1'  : ",a !== '1');
    console.log("a > 1      : ",a > 1);
    console.log("a >= 1     : ",a >= 1);
    console.log("a < 1      : ",a < 1);
    console.log("a <= 1     : ",a <= 1);
}
function ex07(){
    console.log("----- ex07 -----");
    var a = 10;
    var b = 20;
    console.log(a + " + "+b+" = "+(a+b)+" 입니다.");
}

function ex08(text="apple"){
    console.log("----- ex08 -----");
    
    if(text == "apple")
        console.log("사과");
    else if(text == "banana")
        console.log("바나나");
    else   
        console.log("없는 항목 입니다.");
}

function ex10(text){
    console.log("----- ex10 -----");
    for(var i = 0;i<10;i++){
        console.log("i = ",i);
    }
}

function ex15(){
    console.log("----- ex15 -----");
    var array =[1,2,3,4,5];
    for(var i = 0;i<array.length;i++){
        console.log("array["+i+"]: ",array[i]);
    }
}

function ex16(){
    console.log("----- ex16 -----");
    var person = {
        name : ["이름", "성"],
        age : 24,
        fullName : function(){return this.name[0] +' '+ this.name[1]}
    };
    console.log("person.fullName    : ", person.fullName());
    console.log("person.firstName   : ", person.name[0]);
    console.log("person.lastName    : ", person.name[1]);
    console.log("person.age         : ",person.age);
}
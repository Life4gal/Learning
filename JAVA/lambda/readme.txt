
lambda方法格式：
    (参数列表)->{一些重写方法的代码}
    ()：接口中抽象方法的参数列表，没有就为空，有多个用逗号分隔
    ->：传递
    {}：重写接口抽象方法的方法体

lambda表达式是可推导，可省略的
凡是可以根据上下文推导出来的内容，都可以省略书写
可以省略的内容：
    1.(参数列表)：参数列表中参数的数据类型可以省略不写
    2.(参数列表)：参数如果只有一个，那么类型和()都可以省略
    3.{一些代码}：如果{}中的代码只有一行，无论是否有返回值，都可以省略'{}'和'return'和';'
        注意：要省略必须三个一起省略

使用前提：lambda必须要有接口，且接口中有且只有一个抽象方法

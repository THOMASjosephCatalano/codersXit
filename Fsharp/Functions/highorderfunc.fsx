let twice f x = f (f x);;
 
val twice : ('a -> 'a) -> 'a -> 'a
 
> twice System.Math.Sqrt 81.0;;
val it : float = 3.0

//Another example, using an operator as a function: 
> List.map2 (+) [1;2;3] [3;2;1];;
val it : int list = [4; 4; 4]

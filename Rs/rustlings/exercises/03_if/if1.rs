use std::cmp;

/* Bruhhh no ternary
fn bigger3(a: i32, b:i32) -> i32 {
    a >= b ? a : b // Compile ERROR!
}
*/

fn bigger2(a: i32, b:i32) -> i32 {
    if a >= b {
        a
    }
    else {
        b
    }
}

fn bigger(a: i32, b: i32) -> i32 {
    // TODO: Complete this function to return the bigger number!
    // If both numbers are equal, any of them can be returned.
    // Do not use:
    // - another function call
    // - additional variables
    cmp::max(a, b)
}

fn main() {
    println!("{}", bigger2(100, 101));
}

// Don't mind this for now :)
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ten_is_bigger_than_eight() {
        assert_eq!(10, bigger(10, 8));
    }

    #[test]
    fn fortytwo_is_bigger_than_thirtytwo() {
        assert_eq!(42, bigger(32, 42));
    }

    #[test]
    fn equal_numbers() {
        assert_eq!(42, bigger(42, 42));
    }
}

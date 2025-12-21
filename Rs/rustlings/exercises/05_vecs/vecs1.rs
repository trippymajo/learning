fn array_and_vec1() -> ([i32; 4], Vec<i32>) {
    let a = [10, 20, 30, 40]; // Array

    let v = a.to_vec();
    let v = Vec::from(a);
    (a, v)
}

fn array_and_vec() -> ([i32; 4], Vec<i32>) {
    let a = [10, 20, 30, 40]; // Array

    // TODO: Create a vector called `v` which contains the exact same elements as in the array `a`.
    // Use the vector macro.
    // let v = ???;
    let v = vec![a[0], a[1], a[2], a[3]];
    (a, v)
}

fn main() {
    // You can optionally experiment here.
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_array_and_vec_similarity() {
        let (a, v) = array_and_vec();
        assert_eq!(a, *v);
    }
}

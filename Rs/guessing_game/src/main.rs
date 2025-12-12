use std::cmp::Ordering;
use std::io;

use rand::Rng;

fn main() {
    let secret_num = rand::rng().random_range(1..=100);

    println!("Guess the number!");

    let mut guess_buf = String::new();
    loop {
        println!("Please input your guess.");
        
        guess_buf.clear();
        io::stdin()
            .read_line(&mut guess_buf)
            .expect("Failed to read line");
        let guess: u32 = match guess_buf.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        println!("You guessed: {guess}");

        match guess.cmp(&secret_num) {
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        }
    }
}

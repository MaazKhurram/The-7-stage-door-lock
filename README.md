# The-7-stage-door-lock
A door lock with 7 stages of identity verification using micro-controllers.


"The 7-Stage Doorlock" is one of my best hobby projects . I named it "The 7-Stage Doorlock" because it has 7 layers of verification before it allows access to the authorized personnel. The design is modular and an LED screen prompts the user before each verification stage . Also, each security layer is arranged so that the easier-to-pass security verifications are made first (e.g Distance or heat) and the hardest verifications come at the end (e.g Fingerprint). The 7 layers of identification are as follows:

1: Distance (The lock senses if an object comes near it and turns on , otherwise stays on energy-saving mode)

2: Heat (The lock senses if the object emits heat , like humans , but the problem is any combination-generating machine can also come near and emit heat hence satisfying Stage 1 and 2)

3: Pulse detection (This stage is to differentiate between humans and any robot/random machine since only humans can demonstrate an active pulse)

4: CAPTCHA Generation (An LED screen displays a random 8 digit number and the user has to type the same PIN using a keypad mounted on the doorlock . This , again, is to verify that the person is self-thinking and not a robot trying to intrude by repeatedly generating random lock combinations)

5: RFID verification (The authorized personnel are given an RFID tag in form of a keychain or card, which they can tap on the card reader mounted inside the doorlock chasis itself. The RFID identity of all the authorized personnel is maintained on a separate database)

6: Ten-digit PIN (Each person is provided with his personal 10-digit PIN which he has to enter using the keypad)

7: Fingerprint Verification ( The fingerprint sensor matches the fingerprint image of the user with the ones it has stored in the database)

Additional Features:
-Retry after 3 failed attempts at any stage
-Secret 2-step quick access incase of emergency
-Image captured of user after 3 failed attempts
-Doorlock power supply protected by laser tripwire alarm


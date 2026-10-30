# Artificial Space: Vision (draft)

Status: **draft, under discussion.** Nothing here is signed off yet.

## The premise (Lyra's words, lightly organised)

A slow-paced, non-combat space flight game about exploration and
interstellar manufacturing, set in our **real** local star neighbourhood.

- **C++, from the ground up.** No big frameworks. We build the stack ourselves.
- **First-person view**, simple graphics to start.
- **Real data:** a model of our local space built from publicly available
  space agency data (planets, moons, nearby stars).
- **Real Newtonian physics:** no drag, real gravitational influence.
- **Two travel scales:** impulse (interplanetary) and warp (interstellar).
- **Economy:** stars give off energy, which acts as the basic currency used to
  manufacture fuel and basic matter. Some planets offer exotic matter to mine.
  Asteroid mining would be a fun future feature.
- **Building:** manufacturing sites on or off planets. Planets get upgraded and
  get facilities built.
- **Progression:** tech hidden in different places, research that needs you to
  visit certain locations or do certain things (e.g. build a station orbiting a
  star).
- **Tone:** slower paced, no combat (probably), exploration and building.

## Pillars (proposed)

1. **Real space.** The map and the physics are real, and the game's fun comes
   from that.
2. **Understood code.** We build it ourselves and know why every piece exists.
3. **Calm.** No combat. The challenge comes from distance, physics and logistics.

## Decided

- **Libraries:** small, industry-standard libraries for low-level jobs (window,
  input, getting pixels on screen) are fine. No big engines or frameworks
  (Unity, Unreal, etc.). Each library is discussed before it's added.
- **Language:** modern C++, using the language to its fullest. Explain the
  C++ concepts as they come up.
- **Scale:** distances don't have to be literal metres. What matters is that
  positions line up with the real star map, that space *feels* far, and that
  interstellar and interplanetary distances keep their real ratio to each other.
- **On rails:** stars, planets and moons follow predefined paths. Only the
  ship is fully simulated, and it feels the gravity of nearby bodies.
- **Build order:** 1. window, loop, triangle → 2. maths library →
  3. first-person camera and spheres → 4. solar system from real data →
  5. Newtonian ship flight and time acceleration → 6. star map and warp →
  7. economy, manufacturing, research.

## Open design questions

- How does warp coexist with Newtonian physics? (Proposal: warp only works
  outside gravity wells.)
- Time acceleration: almost certainly needed. Settle the details at step 5.
- Precision: if the real interstellar-to-interplanetary ratio is kept
  (~1 AU vs ~4.2 ly ≈ 1 : 266 000), floating-point precision still matters
  whatever units we pick. On rails plus "only the local system matters" makes
  this much easier. Revisit at step 4.
- Is there a "point" or goal, or is it an open sandbox?
- How big is "local space"? A sphere of 10, 25, 50 light years?

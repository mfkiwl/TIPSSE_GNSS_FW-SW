/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./*.{js,mjs,html}",
    "./src/**/*.{js,mjs,html}",
  ],
  theme: {
    extend: {
      fontFamily: {
        'sans': ['Noto Sans TC', 'sans-serif'],
      },
      height: {
        "10v": "10vh",
        "20v": "20vh",
        "30v": "30vh",
        "40v": "40vh",
        "50v": "50vh",
        "60v": "60vh",
        "70v": "70vh",
        "80v": "80vh",
        "90v": "90vh",
        "100v": "100vh",
      },
    },
  },
  plugins: [
    require("daisyui"),
  ],
  daisyui: {
    themes: false,
  },
}

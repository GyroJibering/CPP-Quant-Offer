const root = document.documentElement;
const toggle = document.querySelector<HTMLButtonElement>('#theme-toggle');
const systemTheme = matchMedia('(prefers-color-scheme: dark)');
const storageKey = 'cpp-quant-offer:theme';

function applyTheme() {
  const preference = root.dataset.themePreference;
  const dark = preference === 'dark' || (preference !== 'light' && systemTheme.matches);
  root.dataset.theme = dark ? 'dark' : 'light';
  if (toggle) {
    const label = dark ? '切换到浅色模式' : '切换到深色模式';
    toggle.setAttribute('aria-label', label);
    toggle.title = label;
    toggle.querySelector('.theme-label')!.textContent = dark ? '浅色' : '深色';
    toggle.hidden = false;
  }
}

toggle?.addEventListener('click', () => {
  const theme = root.dataset.theme === 'dark' ? 'light' : 'dark';
  root.dataset.themePreference = theme;
  applyTheme();
  try {
    localStorage.setItem(storageKey, theme);
  } catch {
    // Keep the selection for this page even when storage is unavailable.
  }
});

systemTheme.addEventListener('change', applyTheme);
window.addEventListener('storage', (event) => {
  if (event.key !== storageKey && event.key !== null) return;
  root.dataset.themePreference =
    event.newValue === 'light' || event.newValue === 'dark' ? event.newValue : 'system';
  applyTheme();
});
applyTheme();

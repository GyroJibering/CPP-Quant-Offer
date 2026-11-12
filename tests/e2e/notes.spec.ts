import { test, expect } from '@playwright/test';
import { categories } from '../../src/data/taxonomy.mjs';

test('solver module is discoverable from navigation, search and the knowledge map', async ({
  page,
}) => {
  await page.goto('./?category=optimization');
  await expect(page.locator('#category')).toHaveValue('optimization');
  await expect(page.locator('.topic-row:visible')).toHaveCount(1);
  await expect(page.locator('.topic-row:visible')).toContainText('BarraOpt / MOSEK');
  await page.getByRole('searchbox').fill('KKT');
  await expect(page.locator('.topic-row:visible')).toHaveCount(1);
  await page.locator('.topic-row:visible h2 a').click();
  await expect(page.locator('.prose')).toContainText('相对残差');
  await expect(page.locator('.question')).toHaveCount(21);
  await expect(page.locator('.answer').first()).not.toHaveAttribute('open');
  await page.goto('./roadmap/');
  await expect(page.locator('.intro')).toContainText(`${categories.length} 个领域`);
  const section = page.locator('.roadmap > section').filter({ hasText: '组合优化与求解器' });
  await expect(section.getByRole('link')).toHaveCount(3);
  await expect(section).toContainText('Solver Engineering');
});

test('core notes include Proxy boundaries and both additions are available for interview practice', async ({
  page,
}) => {
  await page.goto('./topics/cpp-quant-core-notes/');
  await expect(page.locator('.prose')).toContainText('weak_ptr');
  await expect(page.locator('.prose')).toContainText('不依赖第三方 Proxy 库');
  await expect(page.locator('.question')).toHaveCount(18);
  await expect(page.locator('.prose a[href="https://github.com/ngcpp/proxy"]')).toBeVisible();
  await page.goto('./interview/');
  for (const topic of ['portfolio-optimization-qd', 'cpp-quant-core-notes']) {
    await page.locator('#interview-topic').selectOption(topic);
    await page.locator('#interview-level').selectOption('L3');
    await page.locator('#start-interview').click();
    await expect(page.locator('#question-level')).toContainText('L3');
    await expect(page.locator('#question-topic')).toHaveAttribute(
      'href',
      new RegExp(`/topics/${topic}/`),
    );
    await expect(page.locator('#interview-answer')).not.toHaveAttribute('open');
  }
});

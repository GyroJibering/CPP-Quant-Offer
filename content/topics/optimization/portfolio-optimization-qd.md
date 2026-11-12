---
{
  'schemaVersion': 1,
  'id': 'portfolio-optimization-qd',
  'title': 'Portfolio Optimization QD：从 BarraOpt / MOSEK 到 KKT、内点法与生产排障',
  'description': '面向组合优化 Quant Developer，贯通收益风险成本建模、凸性与 KKT、原始对偶内点法、稀疏线性系统、MOSEK 状态判读，以及实盘不可行和数值异常的定位流程。',
  'category': 'optimization',
  'areas': ['Portfolio Optimization', 'Numerical Optimization', 'Solver Engineering'],
  'tags':
    [
      'portfolio-optimization',
      'barraopt',
      'mosek',
      'quadratic-programming',
      'kkt',
      'interior-point',
      'numerical-stability',
    ],
  'difficulty': 'L2',
  'roles': ['Quant Developer', 'C++ Developer', 'Trading Infrastructure Engineer'],
  'companyTypes': ['量化私募', 'Trading Firm'],
  'status': 'published',
  'updated': '2026-11-12',
  'reviewed': '2026-11-12',
  'standard': 'C++20',
  'estimatedMinutes': 110,
  'prerequisites':
    [
      '向量与矩阵乘法、转置、导数',
      '基本 C++20、浮点数与断言',
      '投资组合权重、收益率、协方差的含义',
    ],
  'related':
    [
      'benchmark-methodology',
      'cpu-cache-false-sharing',
      'perf-flamegraph',
      'oms-pre-trade-risk',
      'abi-name-mangling',
    ],
  'demo':
    {
      'file': 'examples/portfolio-optimization-qd.cpp',
      'platform': 'portable',
      'exercise': '将 w1 上限从 0.4 改为 0.6，判断活跃约束和乘子符号；核对奇异矩阵负例与相对残差，并为非有限系数增加测试。',
    },
  'references':
    [
      {
        'title': 'Boyd and Vandenberghe: Convex Optimization',
        'url': 'https://web.stanford.edu/~boyd/cvxbook/',
        'kind': 'standard',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MSCI Barra Optimizer on FactSet factsheet',
        'url': 'https://www.msci.com/documents/10199/7bcc8d27-1780-4385-85dd-19fad2667435',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK Optimizer API for C: Accessing the solution',
        'url': 'https://docs.mosek.com/latest/capi/accessing-solution.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK Optimizers: Presolve and algorithm selection',
        'url': 'https://docs.mosek.com/latest/cmdtools/cont-optimizers.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK: Linear optimization, interior-point method and termination',
        'url': 'https://docs.mosek.com/latest/cmdtools/solving-linear.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK: Debugging primal infeasibility',
        'url': 'https://docs.mosek.com/latest/cmdtools/debugging-infeas.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK C API: Retrieving infeasibility certificates',
        'url': 'https://docs.mosek.com/latest/capi/tutorial-pinfeas-shared.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK: Addressing numerical issues',
        'url': 'https://docs.mosek.com/latest/pythonapi/debugging-numerical.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK: Understanding optimizer log',
        'url': 'https://docs.mosek.com/latest/cmdtools/debugging-log.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'MOSEK: Technical FAQ, threads and hot-start',
        'url': 'https://docs.mosek.com/latest/faq/faq.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
      {
        'title': 'IBM CPLEX Optimizer',
        'url': 'https://www.ibm.com/products/ilog-cplex-optimization-studio/cplex-optimizer',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
    ],
  'questions':
    [
      {
        'id': 'portfolio-optimization-qd-q01',
        'level': 'L1',
        'prompt': '如何解释组合优化中 alpha 的线性收益项和协方差的二次风险项？',
        'answer': 'alpha^T w 是各资产预测超额收益按组合权重求和；w^T Sigma w 是组合收益率方差，展开后既包括单资产方差项，也包括两两资产协方差的交叉项。二者必须使用一致的收益率时间尺度。',
        'rubric': ['解释收益加权和', '解释方差及交叉协方差', '提及单位或时间尺度'],
        'source':
          { 'kind': 'derived', 'rationale': '由组合收益线性性与协方差双线性展开直接推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q02',
        'level': 'L1',
        'prompt': '什么情况下标准二次投资组合目标是凸 QP？',
        'answer': '将收益最大化改写为最小化后，目标二次项 Hessian 为半正定矩阵，线性项不破坏凸性；同时线性等式、不等式形成凸可行域。仅有二次表达式并不保证凸，必须检查 Hessian 与其他约束。',
        'rubric': ['Hessian 半正定', '线性项不影响凸性', '可行域凸'],
        'source': { 'kind': 'derived', 'rationale': '由二次函数的 Hessian 判据和凸优化定义推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q03',
        'level': 'L1',
        'prompt': 'LP、QP、SOCP 与 MIP 的建模能力有什么主要区别？',
        'answer': 'LP 使用线性目标与线性约束；凸 QP 允许半正定二次目标；SOCP 能表达二阶锥形式的范数和一些风险约束；MIP 引入整数或二元决策，通常需要离散搜索，不能直接套用连续凸问题的局部最优保证。',
        'rubric': ['区分目标和约束形式', '说明 SOCP 范数约束', '说明整数变量影响'],
        'source': { 'kind': 'derived', 'rationale': '由各优化问题标准形式及变量域定义推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q04',
        'level': 'L1',
        'prompt': '什么是 hard constraint，什么是 soft penalty？',
        'answer': '硬约束要求每个有效结果都满足给定边界；软惩罚把违反偏好或换手大小计入目标函数，允许优化器在收益与惩罚之间权衡。改变惩罚系数通常不会解除与之无关的硬约束冲突。',
        'rubric': ['硬约束决定可行集', '惩罚项影响目标值', '参数与可行性区分'],
        'source': { 'kind': 'derived', 'rationale': '由约束集合与目标函数的不同数学角色推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q05',
        'level': 'L1',
        'prompt': 'KKT 的四个条件分别是什么？',
        'answer': '它们是原始可行性、对偶可行性、驻点条件和互补松弛。对于可微凸问题，满足这些条件可证明全局最优；要保证最优解必有相应乘子，需要相应的约束资格条件。',
        'rubric': ['准确列四项', '说明最优性方向', '提及约束资格限制'],
        'source':
          { 'kind': 'derived', 'rationale': '由可微约束优化的 KKT 系统及凸充分性定理推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q06',
        'level': 'L1',
        'prompt': 'Primal Infeasible、Dual Infeasible 和 Unknown 有什么区别？',
        'answer': 'Primal Infeasible 表示原始约束被证明没有共同可行解；Dual Infeasible 通常对应原始无界的证明方向，但应看具体证书与模型类别；Unknown 只表示尚不能给出要求的可靠结论，不等于已证明不可行。',
        'rubric': ['原始不可行的数学含义', '双重不可行的适用边界', 'Unknown 不等于 Infeasible'],
        'source': { 'kind': 'derived', 'rationale': '由最优化可行性与 MOSEK 官方状态语义归纳。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q07',
        'level': 'L1',
        'prompt': 'MOSEK 的 response code、termination code、problem status 和 solution status 等价吗？',
        'answer': '不等价。Response code 反映 API 错误，termination code 表示求解为何终止；problem status 描述问题的可行性结论，solution status 描述选定解是最优解、证书或未知。MSK_RES_OK 本身不能证明找到了可用最优解。',
        'rubric': ['区分四类状态', '明确 MSK_RES_OK 限制', '知道要选择解类型'],
        'source': { 'kind': 'derived', 'rationale': '由 MOSEK C API 的返回码与解状态设计推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q08',
        'level': 'L2',
        'prompt': '为什么凸优化问题的局部最小值也是全局最小值？',
        'answer': '若存在比局部最优点 x 更好的可行点 y，由凸可行域可取任意接近 x 的线段点 z；凸性保证 f(z) 小于 f(x)，与局部最小定义矛盾。前提同时包括凸目标与凸可行域，严格凸并非必须。',
        'rubric': ['线段可行', '凸不等式给出改进点', '反证局部最优'],
        'source':
          { 'kind': 'derived', 'rationale': '由凸函数 Jensen 不等式与凸可行域闭合于线段推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q09',
        'level': 'L2',
        'prompt': '为什么相关性和协方差矩阵的非对角元不能随意忽略？',
        'answer': '组合方差展开包含 2*w_i*w_j*Cov(i,j) 项；忽略它相当于假定不同资产风险独立，可能严重低估共同方向暴露或错过对冲效果。只有模型假设和数据支持时，才能采用对角近似。',
        'rubric': ['给出交叉项', '解释同涨同跌或对冲', '说明近似前提'],
        'source': { 'kind': 'derived', 'rationale': '由 w^T Sigma w 的双重求和展开推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q10',
        'level': 'L2',
        'prompt': '如何把 turnover 的绝对值约束转成 LP/QP 可用的线性约束？',
        'answer': '为每个资产引入 t_i，添加 t_i>=w_i-w0_i、t_i>=-(w_i-w0_i)、t_i>=0，再限制 sum(t_i)<=T；求解器因而能在线性可行域内表达 L1 换手上界，额外增加变量和约束。',
        'rubric': ['引入非负辅助变量', '两侧差额线性不等式', '总和上界'],
        'source': { 'kind': 'derived', 'rationale': '由绝对值的 epigraph 线性化构造推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q11',
        'level': 'L2',
        'prompt': '等式约束 QP 的 KKT 线性系统由哪些块构成？',
        'answer': '对于 1/2*w^TQw+c^Tw、Aw=b，KKT 系统为 [Q A^T; A 0]*[w;lambda]=[-c;b]。Q 是目标 Hessian，A 是约束雅可比；整体常是对称不定系统，即使 Q 半正定也不保证 KKT 矩阵正定。',
        'rubric': ['写出分块矩阵', '标明 Q 与 A', '区分 PSD Q 和 KKT 正定性'],
        'source': { 'kind': 'derived', 'rationale': '由拉格朗日一阶条件与等式约束合并推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q12',
        'level': 'L2',
        'prompt': '原始对偶内点法为什么要引入 slack 和扰动互补条件？',
        'answer': '把 Ax<=b 改写为 Ax+s=b、s>=0，使约束边界由 slack 描述；迭代中保持 s、对偶乘子 z 为正，并将 s_i*z_i=0 暂改为 s_i*z_i=tau>0，逐渐降低 tau，向互补松弛收敛。',
        'rubric': ['松弛变量变换', '正性和中心路径', '扰动参数趋零'],
        'source':
          { 'kind': 'derived', 'rationale': '由原始对偶内点法中心路径的扰动 KKT 系统推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q13',
        'level': 'L2',
        'prompt': '为什么内点法每轮都需要求解大型线性系统？',
        'answer': '驻点、原始可行性和扰动互补条件构成非线性方程组。Newton 法在当前迭代点线性化，计算调整方向 Delta，再进行步长选择和迭代；主要计算成本可能集中在稀疏 Newton/KKT 系统的分解。',
        'rubric': ['KKT 残差与非线性互补', 'Newton 线性化', '稀疏线性求解成本'],
        'source': { 'kind': 'derived', 'rationale': '由扰动 KKT 线性化和 Newton 迭代方程推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q14',
        'level': 'L2',
        'prompt': '什么是 Duality Gap，它怎样帮助判断最优性？',
        'answer': '对最小化问题，有效原始可行目标值给上界，有效对偶可行目标值给下界；两者差距越小，最优值所在区间越窄。求解器还需要检查两侧可行性及归一化容差，不能只看一个绝对 gap 数字。',
        'rubric': ['原始上界与对偶下界', 'Gap 非负与趋零', '结合可行性和容差'],
        'source': { 'kind': 'derived', 'rationale': '由弱对偶和凸优化停止判据推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q15',
        'level': 'L3',
        'prompt': '策略股票池只剩 8 只，满仓且每只最多 10%，你如何排障？',
        'answer': '先做模型级容量预检：最大总权重只有 0.8，和总权重等于 1 矛盾，故无可行解。回溯股票池过滤、单票上限和资金约束的生效配置，不应靠提高 MOSEK 迭代次数或隐式放松风控解决。',
        'rubric': ['算出 0.8 小于 1', '追溯输入及约束来源', '不滥改风控'],
        'source':
          { 'kind': 'derived', 'rationale': '由线性上界总和与预算等式的必要可行性条件推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q16',
        'level': 'L3',
        'prompt': 'AUM 减半后策略不可行，你如何区分单位问题和约束冲突？',
        'answer': '保留失败前后模型快照，核对 per-leg 约束到底按金额、股数还是权重计量，再检查当前持仓、目标仓位与单次变动上限。将金额限额除以 AUM 会改变权重限额；确认变换、上下界和过滤结果后再查证书。',
        'rubric': ['保留两个时点的原始模型', '明确金额与权重换算', '定位固定持仓与变动约束冲突'],
        'source': { 'kind': 'derived', 'rationale': '由权重金额换算与限额随 AUM 改变的事实推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q17',
        'level': 'L3',
        'prompt': 'MOSEK 返回 MSK_RES_OK，但没有 Optimal，应用层应该怎么做？',
        'answer': '先获取 termination、problem status、solution status 和对应解类型；只有通过明确的状态检查及独立业务约束校验后，才能将目标权重交给下游。UNKNOWN、证书和意外状态应记录并进入定义好的失败路径，不能盲目读取旧解。',
        'rubric': ['区分 API 成功和求解成功', '明确检查多类状态', '失败闭环阻断下单'],
        'source':
          { 'kind': 'derived', 'rationale': '由 MOSEK C API 的结果获取契约与交易风险边界推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q18',
        'level': 'L3',
        'prompt': '两天模型维度相同、迭代数相近，但求解时间从 0.8 秒增至 5 秒，你如何诊断？',
        'answer': '比较预处理与分解时间、矩阵 nnz、分解 fill-in、浮点运算预估量以及线程、内存和机器负载。相同维度不代表相同稀疏图；用固定输入复现，再分别分析模型构造、数值求解和下游时间，避免只调整线程数。',
        'rubric': ['按阶段计时', '检查稀疏结构与 fill-in', '隔离机器负载和参数'],
        'source': { 'kind': 'derived', 'rationale': '由稀疏消元填充与求解器分阶段性能开销推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q19',
        'level': 'L3',
        'prompt': '为何不能默认把昨天的内点法解当作 MOSEK 今天的 hot start？',
        'answer': 'MOSEK 官方 FAQ 说明其内点优化器不支持通用可靠的 hot-start；今天的模型参数、预处理映射和约束也可能变化。可以缓存已验证输入或评估适用的单纯形 warm-start，但要分别验证状态和收益，不能假定提速。',
        'rubric': ['识别 MOSEK 内点法限制', '模型变化与可行性', '区分单纯形和内点法'],
        'source':
          { 'kind': 'derived', 'rationale': '由 MOSEK 官方 hot-start FAQ 与每日模型变化推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q20',
        'level': 'L3',
        'prompt': '使用 LD_PRELOAD 记录 BarraOpt 到 MOSEK 的符号调用，能推断哪些事情，不能推断哪些？',
        'answer': '可在明确的动态链接与符号可见性前提下记录已拦截 API 的顺序、参数摘要、Task 生命周期和调用次数。无法仅凭一次 trace 证明所有执行路径、内部算法选择或约束语义；需比对导出的模型、状态与多场景实验，并注意静态链接或直接内部调用可能绕过拦截。',
        'rubric':
          ['动态链接拦截的可见边界', '调用序列不等于内部实现证明', '通过模型与场景交叉验证'],
        'source':
          { 'kind': 'derived', 'rationale': '由 ELF 动态符号插桩观察范围与黑盒实验可证伪性推导。' },
        'companies': [],
      },
      {
        'id': 'portfolio-optimization-qd-q21',
        'level': 'L3',
        'prompt': '如何验收一次声称提高 BarraOpt 求解稳定性和性能的改动？',
        'answer': '固定代表性策略输入快照和风险配置，比较求解状态、约束违反、目标值或可证明的 gap，并统计异常率、内存和分阶段 p50/p99 延迟。区分硬件、求解器版本、线程数与数据变化；明确超时与失败回退，不以单次更快或 MSK_RES_OK 当成成功。',
        'rubric': ['正确性与状态先于性能', '可重复控制变量和分布指标', '业务失败路径可验证'],
        'source': { 'kind': 'derived', 'rationale': '由生产求解器质量门槛与可重复基准设计推导。' },
        'companies': [],
      },
    ],
}
---

## 30 秒面试回答

Portfolio Optimization Quant Developer 负责把投资策略的收益目标、风险模型、仓位与交易限制转成可求解的数学模型，并维护从 BarraOpt 等建模接口到 MOSEK 等数值求解器的执行链路。连续凸 QP 常以半正定 Hessian 和凸约束保证局部最优即全局最优；KKT 条件、原始和对偶残差及对偶间隙用于判断解的质量。实盘中还必须区分模型不可行、数值不稳定、API 错误和业务约束违反，保留输入快照、求解状态与模型证据，保证失败结果不会进入下单环节。

## 核心概念

### QD 在组合优化链路中的职责

一个简化的数据流是：策略提供 Alpha、当前持仓、可交易池和风控参数；优化层把它们转成目标函数和约束；数值求解器返回解或失败结论；应用层验证结果，再把目标权重交给交易风控与订单系统。策略预测与交易执行可以由不同团队负责，优化 QD 的接口契约必须明确输入快照、单位、失败语义和可重放能力。

| 对象                       | 工作内容                                    | 易混淆的边界                                                 |
| -------------------------- | ------------------------------------------- | ------------------------------------------------------------ |
| Barra 风险模型             | 根据因子暴露与风险估计生成协方差等输入      | 风险模型本身不等于数值优化算法                               |
| Barra Optimizer / BarraOpt | 组合目标、约束和求解流程的产品或接口层      | 不应仅凭名称假定所有版本调用相同后端                         |
| MOSEK                      | 求解 LP、凸 QP、锥规划等模型的商业求解器    | 不负责判断业务数据是否过时或仓位是否可下单                   |
| CPLEX                      | 另一款商业数学优化求解器，支持 LP/QP/MIP 等 | 能力有交集，不代表一定集成在当前 BarraOpt 中                 |
| MOPS / NOPS                | 在旧系统或特定安装环境中可能出现的名称      | 未核实具体版本的符号、模块或文档前，不把它们写成确定调用后端 |

MSCI 的 [Barra Optimizer on FactSet 资料](https://www.msci.com/documents/10199/7bcc8d27-1780-4385-85dd-19fad2667435) 描述了其组合优化能力；MOSEK 的算法与 API 语义应以 [MOSEK 官方文档](https://docs.mosek.com/latest/cmdtools/cont-optimizers.html) 为准。这些公开资料不足以推出某个部署版本的内部函数顺序。

### 五个基础量：权重、收益、协方差、当前持仓、AUM

记 `w_i` 为第 `i` 个资产的目标组合权重，`alpha_i` 为相同时间范围内的预测收益，`w0_i` 为当前权重，`AUM` 为组合资产规模。金额与权重必须显式换算：

```text
目标持仓金额_i = AUM × w_i
目标交易金额_i = AUM × (w_i - w0_i)   // 假设 AUM 基准一致，无现金流和价格变动
```

预测收益 `alphaᵀw = Σ_i alpha_i × w_i`。组合方差 `wᵀΣw` 则需要完整协方差矩阵 `Σ`。以两只股票为例：

```text
Σ = [ 0.04  0.01 ]     w = [0.5, 0.5]ᵀ
    [ 0.01  0.09 ]

wᵀΣw = 0.25×0.04 + 2×0.5×0.5×0.01 + 0.25×0.09
      = 0.0375
```

非对角项反映两只股票共同波动或对冲的关系。协方差可由历史收益率样本估计，但样本频率、年化口径、缺失值和估计窗口必须统一。方差的单位是收益率平方；波动率才是方差的平方根。实际风险模型还常使用因子形式 `Σ = B F Bᵀ + D`，其中 `B` 为因子暴露，`F` 为因子收益协方差，`D` 为特异风险方差。它的计算结构可以避免始终显式构造完整的稠密 `n×n` 风险矩阵。

### 四类常见数学问题

| 类别       | 目标和约束               | 组合优化例子                 | 重要限制                                 |
| ---------- | ------------------------ | ---------------------------- | ---------------------------------------- |
| LP         | 线性目标与线性约束       | 只考虑线性收益、持仓上下限   | 不能直接表示一般二次风险目标             |
| 凸 QP      | 半正定二次目标与线性约束 | 均值方差优化、二次交易成本   | Hessian 必须半正定才是凸 QP              |
| SOCP       | 线性目标与二阶锥约束     | `‖Lw‖₂ ≤ risk_limit`         | 需要正确的锥形式与缩放                   |
| MIP / MIQP | 存在整数或二元变量       | 持仓数量、离散手数或开仓决策 | 通常需要分支搜索；复杂度与连续凸规划不同 |

SOCP 与凸 QP 有重叠：若 `Σ = LᵀL`，则波动率上限 `sqrt(wᵀΣw) ≤ σmax` 可写成 `‖Lw‖₂ ≤ σmax`。具体建模器如何转化以及调用哪种优化器，需要从实际生成的 Task 或导出模型确认。

## 原理深入

### 从均值方差优化到标准 QP

一类常见组合目标是：

```text
maximize_w  alphaᵀw - λ wᵀΣw - γ · turnover(w, w0) - trading_cost(w-w0)
subject to  sum(w) = 1,  lower ≤ w ≤ upper,  additional risk/trade constraints
```

忽略非二次成本，转换成最小化标准 QP：

```text
minimize_w  (1/2) wᵀQw + cᵀw
subject to  Aw ≤ b,  Ew = d

Q = 2λΣ, c = -alpha   // 原目标的风险项写为 λ wᵀΣw 时
```

`Q` 是 Hessian（二阶导数组成的矩阵）。如果 `λ ≥ 0` 且协方差矩阵 `Σ` 半正定，那么 `Q` 也半正定，这个二次目标是凸函数。协方差矩阵理论上半正定；实际估计、拼接与浮点误差可能导致轻微负特征值，生产中应分析风险估计与矩阵构造问题，而不是默认删除所有负值。

标准化时不能漏掉二次项的 `1/2`：若 SDK 接口的目标定义为 `(1/2) wᵀQw+cᵀw`，向它传入 `λΣ` 和传入 `2λΣ` 得到的权衡不同。这种系数约定问题在跨层 API 调试中很常见。

### 线性化换手与硬约束

总换手常出现绝对值：`T(w) = Σ_i |w_i-w0_i|`。若要施加硬上限 `T(w) ≤ T_max`，可引入每资产非负辅助变量 `t_i`：

```text
t_i ≥ w_i - w0_i
t_i ≥ w0_i - w_i
t_i ≥ 0
sum_i(t_i) ≤ T_max
```

这样可以用线性约束表达换手上限。若把 `γ Σ_i t_i` 放到目标中，它是软惩罚：收益足够高时允许更多交易，但不会自动使其他硬约束失效。换手的“一买一卖算多少”属于业务口径：部分系统采用 `sum(abs(delta_w))`，部分采用其中的一半，需要在策略与求解接口中保持一致。

单票上下限、行业和因子暴露、净敞口通常可以表示为线性约束；风险上限常是二次或锥约束；持仓数量、最小成交手数等离散条件可能引入整数变量或启发式流程。不能由“它调用 MOSEK”直接推出模型始终是连续凸 QP。

### 为什么凸优化局部最优就是全局最优

凸优化包含两个条件：最小化凸函数，且可行域是凸集。对任意两个可行点 `x,y` 与 `0≤t≤1`，中间点 `z=(1-t)x+ty` 必须可行；凸性保证：

```text
f((1-t)x+ty) ≤ (1-t)f(x) + t f(y)
```

反证：若 `x` 是局部最优而 `y` 有更低的函数值，取任意足够小的 `t>0`，中间点 `z` 可以无限接近 `x`，又满足 `f(z)<f(x)`，与局部最优矛盾。此结论不要求严格凸；例如常数目标可以有无穷多个全局最优。若约束是不连续可行的离散持仓集合，则不能沿线段移动，证明失效。参见 [Boyd 与 Vandenberghe 的教材](https://web.stanford.edu/~boyd/cvxbook/)。

### 拉格朗日函数和 KKT 四个条件

对可微问题 `min f(x)`，约束 `g_i(x)≤0` 与 `h_j(x)=0`，拉格朗日函数为：

```text
L(x, λ, ν) = f(x) + Σ_i λ_i g_i(x) + Σ_j ν_j h_j(x)
```

其中不等式乘子 `λ_i≥0`，等式乘子 `ν_j` 无符号限制。KKT 条件如下：

| 条件       | 数学形式                 | 业务直观                 |
| ---------- | ------------------------ | ------------------------ |
| 原始可行性 | `g_i(x)≤0, h_j(x)=0`     | 持仓及风险约束没有被违反 |
| 对偶可行性 | `λ_i≥0`                  | 不等式乘子满足符号约定   |
| 驻点条件   | `∇f+Σλ_i∇g_i+Σν_j∇h_j=0` | 目标梯度与约束梯度平衡   |
| 互补松弛   | `λ_i g_i(x)=0`           | 未触及边界的约束乘子为零 |

例：`min (x-3)²`，约束 `x≤2`。最优 `x=2`，拉格朗日函数 `L=(x-3)²+λ(x-2)`，驻点条件 `2(x-3)+λ=0` 给 `λ=2`，四个条件均成立。该乘子对应微小放宽上界的边际改善：在适用的光滑性条件下，约束上限增加 `ε`，最优目标值的一阶变化约为 `-2ε`。这是 Shadow Price（影子价格）的一种解释。

对于可微凸目标、凸不等式与仿射等式，满足 KKT 的点是全局最优；把“最优解一定满足 KKT”作为必要结论时还需检查约束资格条件，例如 Slater 条件。实际求解器通过有限容差近似验证条件，不可能要求每个浮点残差都精确为零。

### 对偶性、Duality Gap 与停止条件

对一个最小化问题，原始可行解的目标值给出最优值的上界；对偶可行解给出下界，二者之差称为对偶间隙。若原始值为 `100.001`、对偶值为 `100.000`，绝对 Gap 为 `0.001`。但只看这一个差值不够，必须先检查原始与对偶可行性，且在数值判定中使用相对或缩放后的容差。

读 MOSEK 迭代日志时，`PFEAS`、`DFEAS`、`GFEAS` 分别帮助观察原始可行、对偶可行和间隙相关残差；不同算法或标准化模型下的具体定义以版本文档为准。`POBJ` 与 `DOBJ` 是原始、对偶目标值，`MU` 则反映内点迭代中的互补量级。参阅 [MOSEK 线性优化停止条件](https://docs.mosek.com/latest/cmdtools/solving-linear.html)。

### 内点法：把非线性 KKT 条件逐步线性化

对 `Ax≤b` 引入松弛变量 `s=b-Ax≥0`，并使用非负乘子 `z≥0`。互补条件为 `s_i z_i=0`。原始对偶内点法用正数 `τ` 暂时替换它：

```text
Qx + c + Aᵀz = 0    // 驻点（仅考虑不等式时）
Ax + s - b = 0      // 原始等式
s_i z_i = τ > 0     // 扰动互补条件
```

`τ` 逐步减小时，迭代点向原始互补条件靠近。Newton 法把当前非线性方程一阶线性化。例如 `(s+Δs)(z+Δz)=τ` 忽略二阶小量 `ΔsΔz` 后成为 `zΔs+sΔz=τ-sz`。将所有变量的线性化方程合并，就得到一个用于求调整方向的线性系统。真实 MOSEK 内点算法可能使用齐次原始对偶模型和额外代数变换，不能把本节教学形式误认为它导出的精确内部矩阵。

## 数据结构/系统内部实现

### KKT 矩阵和稀疏分解

只有等式约束的凸 QP `min (1/2)xᵀQx+cᵀx, Ax=b`，其 KKT 条件可写成：

```text
[ Q  Aᵀ ] [ x ] = [ -c ]
[ A   0 ] [ ν ]   [  b ]
```

`Q` 为目标 Hessian，`A` 为等式约束矩阵，`ν` 为等式乘子。即使 `Q` 半正定，整个 KKT 分块矩阵也可能不定。添加不等式和互补条件后，Newton 系统还会包含 slack、对偶变量以及当前迭代状态。解线性系统时一般使用矩阵分解，不显式求矩阵逆。

在几千资产规模下，约束矩阵的绝大多数位置可能为零。CSR/CSC 通过非零值和行列索引存储稀疏结构，减少存储与访问；具体计算库常根据分解方向选择合适表示。稀疏分解会产生额外的非零元素，称为 Fill-in。重排序可以减少 Fill-in，但输入矩阵非零元素相同数量不代表分解成本相同：非零元素所在的位置同样重要。

| 环节                  | 观察什么                             | 典型故障迹象               |
| --------------------- | ------------------------------------ | -------------------------- |
| 数据检查              | 维度、非零系数、上下界、NaN/Inf、PSD | 股票映射错位、单位突然变化 |
| Presolve              | 固定变量、冗余约束、线性相关性       | 模型预处理时间显著变长     |
| Scaling / Dualization | 系数量级、原始或对偶模型选择         | 残差停滞、状态 Unknown     |
| Newton/KKT 分解       | 非零元素、Fill-in、预估 FLOPs、耗时  | 迭代数相似而耗时暴涨       |
| 终止与解提取          | 原始/对偶残差、Gap、解类型           | API 正常但结果不是 Optimal |

MOSEK 的优化器日志包含预处理、迭代与解摘要；部分日志还输出分解前后非零元素数及运算量估计。算法可以对内部模型做消元、缩放、转对偶等优化，所以应用层看到的 `n` 和 `m` 未必直接等于内部线性系统大小。

### Solver API 状态机与黑盒排查

使用 MOSEK C API 时，典型过程是创建 Environment/Task、填入目标和约束、配置参数、调用优化、查询状态与读取变量。`MSK_optimizetrm` 可以区分 API response 与终止原因，之后还需要按解类型读取 `MSK_getprosta` 与 `MSK_getsolsta`。`MSK_SOL_ITR`、`MSK_SOL_BAS`、`MSK_SOL_ITG` 是不同类型的结果；不能不加判断地从一种解类型读取另一种算法的解。

| 结果类别                  | 应用层处理原则                                             |
| ------------------------- | ---------------------------------------------------------- |
| `MSK_RES_OK`              | 表示没有对应的 API 异常，仍要检查求解状态                  |
| `MSK_SOL_STA_OPTIMAL`     | 进一步复算业务硬约束、金额与舍入后的交易限制               |
| `MSK_PRO_STA_PRIM_INFEAS` | 导出模型，查约束冲突与不可行性证书                         |
| `MSK_PRO_STA_DUAL_INFEAS` | 调查原始无界的可能性及相应证书，不机械等同于任意模型都无界 |
| `MSK_SOL_STA_UNKNOWN`     | 查终止原因、残差、模型量级、迭代停滞与超时                 |

实际程序还必须处理内存耗尽、许可证、非有限输入、模型构造失败和超时。状态检查应在取解之前完成，不能把一个正常的函数返回码当成“今天可下单”。参见 [MOSEK C API 状态文档](https://docs.mosek.com/latest/capi/accessing-solution.html)。

通过 `LD_PRELOAD` / 动态符号代理记录 BarraOpt 与 MOSEK 之间的调用，可以观测成功拦截到的函数、参数摘要、调用次序、Task 创建与销毁、外层异常处理。它不能证明所有未触发路径的行为；静态链接、隐藏符号、内部直接调用或不同版本加载方式都可能影响可见性。想确认“某项约束最终怎样进入 MOSEK”，应把 API trace、导出的 Task/模型、求解结果以及策略层配置一起对照。

## C++ runnable demo

示例手写一个有部分主元选取的微型稠密线性系统求解器，然后验证两只资产的 KKT 系统。首先求 `min (w1²+w2²)/2` 且 `w1+w2=1`，得到 `w1=w2=0.5`；再添加 `w1≤0.4` 并假定上界活跃，求得 `w1=0.4,w2=0.6`，用非负约束乘子和互补松弛检查该候选点确实满足 KKT。最后对 8 只股票各 10% 上限的满仓需求做一个不可能性预检。

```cpp include=examples/portfolio-optimization-qd.cpp

```

```bash
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic -Werror examples/portfolio-optimization-qd.cpp -o /tmp/portfolio-optimization-qd
/tmp/portfolio-optimization-qd
```

预期输出：

```text
equality optimum: 0.5000, 0.5000; objective=0.2500
capped optimum: 0.4000, 0.6000; multiplier=0.2000; objective=0.2600
8 assets with 10% caps and 100% budget: infeasible by bounds
```

示例还检查相对残差，拒绝奇异矩阵和不匹配的维度，并核对上限放宽到 0.6 时原等式解已可行。示例使用 C++20，平台可移植，断言不可禁用。这个有限维稠密程序用于手算和 KKT 检验：它没有实现内点法、通用活跃集搜索、稀疏分解、不可行性证书或完整精度控制。给定活跃约束后再解 KKT 不能充当一般不等式 QP 求解器。大型模型应使用经过验证的数值库及业务结果校验，不能把示例中的相对主元阈值 `1e-12 × max(abs(A_ij))`当作生产通用容差。

## 高频追问

### Hessian 半正定为什么重要，正定与半正定一样吗？

对于二次函数，Hessian 半正定保证目标凸；正定意味着目标严格凸，若存在最优解则最优权重唯一。半正定允许多个不同权重达到同一最优目标。实际风险矩阵可能存在相关因子造成的零特征方向，这不自动意味着模型失败，但可能影响解的唯一性和数值稳定性。

### 约束的影子价格能直接解释成收益率吗？

不能。乘子量纲由目标函数和约束单位决定。目标是收益减风险惩罚时，乘子体现的是目标值对约束右端的边际变化，不一定是现金收益，更不等于放宽任何大小都能线性兑现。多个活跃约束、不可微点和大幅调参都可能改变局部敏感度。

### 为什么调小风险惩罚系数不能修复所有 Infeasible？

风险惩罚系数改变目标偏好；若硬约束彼此矛盾，即使目标函数完全去掉，可行域仍为空。最稳妥的初步实验之一是先单独求可行性，定位是模型构造冲突还是目标、缩放带来的数值问题。删除或放宽硬风控前必须得到明确的业务授权。

### 原始问题无界与原始问题不可行是一回事吗？

不是。不可行表示没有任何满足全部约束的点；无界表示存在一系列可行点，让最小化目标无限下降。对偶状态或相应证书有助于区分，但需要结合模型类和求解器实际报告，不能单看一个中文“求解失败”异常。

### 既然昨天求出了权重，能否今天用 MOSEK 内点法热启动？

MOSEK 官方 FAQ 说明其内点法没有通用可靠的 Hot-start 支持；线性问题使用单纯形法时可能利用现有基解。组合优化模型每天变化，昨天的权重今天也未必满足约束。优化点应是建模复用、输入缓存、求解器算法选择与实测，而不是先许诺热启动提速。

### 增加 CPU 线程是否一定降低求解延迟？

不一定。线程管理、同步、内存带宽、分解并行度和问题规模共同决定吞吐与尾延迟。MOSEK 官方建议用实际模型选择线程数；小型模型有时单线程和高主频处理器更合适。单个 Task 的延迟与批量同时求解多个 Task 的系统吞吐要分别测量。

## 容易答错的点

- `Σ` 是每只股票的波动率列表。错误：协方差矩阵包含资产之间的交叉协方差，`wᵀΣw` 是组合**方差**。
- 任何 QP 都是凸优化。错误：二次 Hessian 可能不定，非凸二次约束或整数变量还可能破坏凸性。
- 凸问题一定只有一个最优解。错误：非严格凸目标可以有多个最优点，但局部最优仍是全局最优。
- KKT 只有驻点 `∇L=0`。错误：还需要原始可行、对偶可行、互补松弛，以及对应的适用前提。
- 单票 10% 上限下有 8 只可买股票仍可以强制满仓。错误：可配置总权重最多 80%。
- `MSK_RES_OK` 等于找到交易可用的最优解。错误：必须检查结果类型与状态，随后做独立业务验证。
- `UNKNOWN` 说明数学模型已被证明不可行。错误：数值不稳定、停滞、提前终止都可能产生未知状态。
- 输入稀疏矩阵的 nnz 不变，求解速度就不变。错误：稀疏结构分布和 Fill-in 会改变分解开销。
- 增大线程数、放宽容差、提高迭代数一定能解决生产故障。错误：这些动作需要基于已定位的瓶颈和正确性风险评估。
- 观察到一次 BarraOpt 到 MOSEK 的 API 调用，就证明所有策略都是一次求解。错误：动态 trace 只覆盖触发的执行路径。

## 性能分析

优化 QD 应分别记录模型准备、Task 构造、Presolve、数值迭代、解提取和业务验证的时间。总耗时不能简单等同于内点法迭代次数；维度相同的两个模型也可能具有不同稀疏结构、缩放特征和分解成本。

| 指标                               | 为什么要测                       | 最少控制变量                 |
| ---------------------------------- | -------------------------------- | ---------------------------- |
| 变量/约束、矩阵 nnz                | 判断模型是否悄悄膨胀             | 相同股票池与约束版本         |
| Presolve 时间                      | 发现固定变量、冗余约束等处理开销 | 同一求解器版本与参数         |
| 迭代数、PFEAS/DFEAS/GFEAS          | 观察收敛质量与停滞               | 相同容差、同一模型           |
| Factor nonzeros / FLOPs            | 追查 Fill-in 与矩阵分解的成本    | 相同稀疏排序与线程设置       |
| 求解 wall time、CPU time、内存峰值 | 区分计算与资源竞争               | 机器负载、NUMA、CPU 频率     |
| p50/p99/p99.9、超时率              | 量化盘中尾延迟与失败             | 固定代表性输入分布及运行环境 |

朴素稠密 `n×n` 矩阵元素存储为 O(n²)，常见稠密分解代价为 O(n³)；稀疏分解成本由结构与 Fill-in 决定，不能给出与 nnz 简单线性相关的普适时间上界。成本还与模型是否采用低秩因子结构、是否引入大量辅助变量及数值问题有关。本文没有针对 BarraOpt 或 MOSEK 的生产环境实测，不提供固定速度或加速倍数。

数值稳定性优先从模型入手：比较目标、约束矩阵与上下界的数量级，排查金额和权重混用、极大伪无穷边界、近线性相关、明显病态问题。MOSEK 官方建议优先通过重构或缩放模型解决数值问题，而不是首先放宽终止容差。详见 [MOSEK 数值问题排查](https://docs.mosek.com/latest/pythonapi/debugging-numerical.html)。

## Quant/Low-Latency 场景

### 实盘排障清单

针对求解失败，先为一次请求生成可复放的输入快照，包含策略版本、交易日、时间戳、股票顺序与 ID 映射、Alpha、风险矩阵或因子参数、当前权重、AUM、目标函数系数、所有约束边界和求解器配置。注意脱敏与权限控制，避免把真实组合或策略数据写到公开日志。

1. **先看 API 与状态**：分清输入错误、许可证或资源错误、超时、`PRIM_INFEAS`、`DUAL_INFEAS`、`UNKNOWN`。
2. **验证生成模型**：检查变量数、约束数、目标符号、系数范围、有限性、上下界和约束索引；必要时导出独立可重放的模型文件。
3. **检查不可行性**：先做可计算的必要条件预检，如上界总和与预算约束、当前持仓是否被冲突的交易约束锁死；再用不可行证书和冲突子问题定位具体约束。
4. **检查数值问题**：比较失败与成功请求的 Scaling、近线性依赖、KKT/分解信息、残差、迭代日志；固定模型复现后才尝试不同参数。
5. **决定业务回退**：失败不得凭旧内存中的权重继续下单。是否维持持仓、停止该轮、使用已经批准的保守目标或升级人工处理，必须由交易系统的明确策略与风控授权决定。

### AUM 与 per-leg 的典型陷阱

假设交易金额上限为 `1,000,000`，组合 AUM 从 `10,000,000` 降为 `5,000,000`。若金额限制固定，上限占 AUM 的比例从 `10%` 变为 `20%`。如果系统直接把“per-leg 0.1”解释成权重变化上限，两天得到的限制不同。还要区分 `abs(target-current)`、只买入上限、只卖出上限，以及单位是金额、权重还是股数。对单次调仓施加 `|w_i-w0_i|≤0.1` 时，可交易目标会受当前持仓影响；若另一组等式要求无法通过这些有限调整达到的暴露，就会真正不可行。

### 不可行性证书与自动定位

对连续问题，MOSEK 可以给出 Farkas 类不可行性证书，对应的约束乘子可以组合出数学上的矛盾。MOSEK 支持自动不可行性报告参数 `MSK_IPAR_INFEAS_REPORT_AUTO`；它指出涉及矛盾的约束与变量边界，适合与策略配置 ID 做映射。证书受浮点误差和预处理影响，并不等于系统已经自动知道哪条业务规则应该删。参阅 [官方报告教程](https://docs.mosek.com/latest/cmdtools/debugging-infeas.html)和 [C API 取证书示例](https://docs.mosek.com/latest/capi/tutorial-pinfeas-shared.html)。

### 如何把黑盒排查转成可复现工程结果

对 BarraOpt 与 MOSEK 的插桩日志，给每个优化请求关联 `request_id` 和 Task 生命周期；日志记录版本、线程、输入哈希、模型维度、参数、API 返回与终止结果。仅用 API 顺序推测算法路径时要注明是推断。若一个 demo 中观察到一次 MOSEK 调用，只能说明该输入走过这条路径；复杂目标、不同约束或异常恢复可能触发不同路径，必须补对照实验。最终性能回归既要比较返回状态和可行性，也要比较目标值、残差、迭代过程及分阶段耗时。

## 相关专题

- [Benchmark methodology](../performance/benchmark-methodology.md)：控制变量、重复次数、吞吐与尾延迟。
- [Perf 与 Flamegraph](../performance/perf-flamegraph.md)：定位 CPU 与系统级耗时，补充求解器自身日志。
- [CPU Cache / False Sharing](../performance/cpu-cache-false-sharing.md)：理解稀疏访问、共享内存和并行成本。
- [OMS / Pre-trade Risk](../trading/oms-pre-trade-risk.md)：目标权重进入订单系统前的硬风控与失败边界。
- [ABI 与 Name Mangling](../cpp/abi-name-mangling.md)：分析 `.so`、动态符号和 API 插桩的前置知识。

## 分层面试题

L1 题首先要说清 `wᵀΣw`、QP、凸性、KKT 和 MOSEK 状态的定义；L2 题要求能手推绝对值线性化、对偶间隙、KKT 矩阵与内点法的 Newton 方程；L3 题要求从 AUM、交易约束、日志和矩阵结构推导一个可复现的排障实验。建议先手算 C++ 示例的两个 KKT 系统，再分别描述 `PRIM_INFEAS` 与 `UNKNOWN` 的不同响应流程。
